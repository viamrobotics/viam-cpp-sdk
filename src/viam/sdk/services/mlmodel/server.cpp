// Copyright 2023 Viam Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <viam/sdk/services/mlmodel/server.hpp>

#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/services/mlmodel/mlmodel.hpp>
#include <viam/sdk/services/mlmodel/private/proto.hpp>

namespace viam {
namespace sdk {

MLModelServiceServer::MLModelServiceServer()
    : MLModelServiceServer(std::make_shared<ResourceManager>()) {}

MLModelServiceServer::MLModelServiceServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

void MLModelServiceServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

::grpc::Status MLModelServiceServer::Infer(
    ::grpc::ServerContext* context,
    const ::viam::service::mlmodel::v1::InferRequest* request,
    ::viam::service::mlmodel::v1::InferResponse* response) {
    if (!request) {
        return {::grpc::StatusCode::INVALID_ARGUMENT, "Called [Infer] without a request"};
    };

    std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());

    if (!rb) {
        return {::grpc::UNKNOWN, "resource not found: " + request->name()};
    }

    std::shared_ptr<MLModelService> mlms = std::dynamic_pointer_cast<MLModelService>(rb);

    if (!request->has_input_data()) {
        return {::grpc::StatusCode::INVALID_ARGUMENT, "Called [Infer] with no input data"};
    }

    // TODO: Metadata should be cached client side so we don't need to
    // do two round trips for every inference.
    const auto md = mlms->metadata();

    mlmodel_details::tensor_storage input_storage;
    MLModelService::named_tensor_views inputs;

    const auto& input_fields = request->input_data().fields();
    for (const auto& input : md.inputs) {
        const auto where = input_fields.find(input.name);

        // Ignore any inputs for which we don't have metadata, since
        // we can't know what type they should decode to.
        //
        // TODO: Should this be an error? For now we just don't decode
        // those tensors.
        if (where == input_fields.end()) {
            continue;
        }

        auto status = mlmodel_details::pb_value_to_tensor(input, where->second, &input_storage, &inputs);
        if (!status.ok()) {
            return status;
        }
    }

    // TODO: Should we handle exceptions here? Or is it ok to let them
    // bubble up and have a higher layer deal with it? Perhaps all of
    // our server side overrides should be `noexcept`.
    const auto outputs = mlms->infer(inputs);
    auto& pb_output_data_fields = *(response->mutable_output_data()->mutable_fields());
    for (const auto& kv : *outputs) {
        auto emplace_result = pb_output_data_fields.try_emplace(kv.first);
        // XXX ACM TODO: check result of emplace
        auto status = mlmodel_details::tensor_to_pb_value(kv.second, &emplace_result.first->second);
        if (!status.ok()) {
            return status;
        }
    }

    return ::grpc::Status();
}

::grpc::Status MLModelServiceServer::Metadata(
    ::grpc::ServerContext* context,
    const ::viam::service::mlmodel::v1::MetadataRequest* request,
    ::viam::service::mlmodel::v1::MetadataResponse* response) {
    if (!request) {
        return {::grpc::StatusCode::INVALID_ARGUMENT, "Called [Metadata] without a request"};
    };

    std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return {grpc::UNKNOWN, "resource not found: " + request->name()};
    }

    std::shared_ptr<MLModelService> mlms = std::dynamic_pointer_cast<MLModelService>(rb);
    auto md = mlms->metadata();

    auto& metadata_pb = *response->mutable_metadata();
    *metadata_pb.mutable_name() = std::move(md.name);
    *metadata_pb.mutable_type() = std::move(md.type);
    *metadata_pb.mutable_description() = std::move(md.description);

    const auto pack_tensor_info = [](auto& target,
                                     const std::vector<MLModelService::tensor_info>& source) {
        target.Reserve(source.size());
        for (auto&& s : source) {
            auto& new_entry = *target.Add();
            *new_entry.mutable_name() = std::move(s.name);
            *new_entry.mutable_description() = std::move(s.description);
            *new_entry.mutable_data_type() = MLModelService::tensor_info::data_type_to_string(s.data_type);
            auto& shape = *new_entry.mutable_shape();
            shape.Reserve(s.shape.size());
            shape.Assign(s.shape.begin(), s.shape.end());
            auto& associated_files = *new_entry.mutable_associated_files();
            associated_files.Reserve(s.associated_files.size());
            for (auto&& af : s.associated_files) {
                auto& new_af = *associated_files.Add();
                *new_af.mutable_name() = std::move(af.name);
                *new_af.mutable_description() = std::move(af.description);
                switch (af.label_type) {
                    case MLModelService::tensor_info::file::k_label_type_tensor_value:
                        new_af.set_label_type(
                            ::viam::service::mlmodel::v1::LABEL_TYPE_TENSOR_VALUE);
                        break;
                    case MLModelService::tensor_info::file::k_label_type_tensor_axis:
                        new_af.set_label_type(::viam::service::mlmodel::v1::LABEL_TYPE_TENSOR_AXIS);
                        break;
                    default:
                        break;
                        // XXX ACM TODO
                }
            }
            *new_entry.mutable_extra() = map_to_struct(s.extra);
        }
    };

    pack_tensor_info(*metadata_pb.mutable_input_info(), md.inputs);
    pack_tensor_info(*metadata_pb.mutable_output_info(), md.outputs);

    return ::grpc::Status();
}

}  // namespace sdk
}  // namespace viam
