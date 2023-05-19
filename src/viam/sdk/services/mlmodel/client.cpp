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

#include <viam/sdk/services/mlmodel/client.hpp>

#include <viam/sdk/services/mlmodel/private/proto.hpp>

#include <grpcpp/channel.h>

namespace viam {
namespace sdk {

namespace {

struct tensor_storage_and_views {
    mlmodel_details::tensor_storage storage;
    MLModelService::named_tensor_views views;
};

}  // namespace

MLModelServiceClient::MLModelServiceClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : MLModelServiceClient(std::move(name), service_type::NewStub(channel)) {
    channel_ = std::move(channel);
}

MLModelServiceClient::MLModelServiceClient(std::string name,
                                           std::unique_ptr<service_type::StubInterface> stub)
    : MLModelService(name), stub_(std::move(stub)) {}

std::shared_ptr<MLModelService::named_tensor_views> MLModelServiceClient::infer(
    const named_tensor_views& inputs) {
    namespace pb = ::google::protobuf;
    namespace mlpb = ::viam::service::mlmodel::v1;

    pb::Arena arena;
    auto* const req = pb::Arena::CreateMessage<mlpb::InferRequest>(&arena);

    req->set_name(this->name());

    auto& mutable_input_data = *req->mutable_input_data();
    auto& mutable_input_data_fields = *mutable_input_data.mutable_fields();

    for (const auto& kv : inputs) {
        auto ib = mutable_input_data_fields.emplace(kv.first);
        pb::Value& value = ib.first->second;
        mlmodel_details::tensor_to_pb_value(kv.second, &value);
    }

    auto* const resp = pb::Arena::CreateMessage<mlpb::InferResponse>(&arena);

    grpc::ClientContext ctx;
    auto result = stub_->Infer(&ctx, *req, resp);

    // TODO: Metadata caching.
    const auto md = metadata();

    auto tsav = std::make_shared<tensor_storage_and_views>();
    const auto& output_fields = resp->output_data().fields();
    for (const auto& output : md.outputs) {
        const auto where = output_fields.find(output.name);
        // Ignore any outputs for which we don't have metadata, since
        // we can't know what type they should decode to.
        if (where != output_fields.end()) {
            mlmodel_details::pb_value_to_tensor(
                output, where->second, &tsav->storage, &tsav->views);
        }
    }

    return {std::move(tsav), &tsav->views};
}

struct MLModelService::metadata MLModelServiceClient::metadata() {
    // Encode metadata args into a `MetadataRequest`
    viam::service::mlmodel::v1::MetadataRequest req;
    *req.mutable_name() = name();

    // Invoke the stub
    grpc::ClientContext ctx;
    viam::service::mlmodel::v1::MetadataResponse resp;
    const auto stub_result = stub_->Metadata(&ctx, req, &resp);

    struct metadata result;
    if (resp.has_metadata()) {
        auto& metadata_pb = *resp.mutable_metadata();
        result.name = std::move(*metadata_pb.mutable_name());
        result.type = std::move(*metadata_pb.mutable_type());
        result.description = std::move(*metadata_pb.mutable_description());

        const auto unpack_tensor_info = [](std::vector<tensor_info>& target, auto& source) {
            target.reserve(source.size());
            for (auto&& s : source) {
                target.emplace_back();
                auto& ti = target.back();
                ti.name = std::move(*s.mutable_name());
                ti.description = std::move(*s.mutable_description());
                auto data_type = MLModelService::tensor_info::string_to_data_type(s.data_type());
                if (!data_type) {
                    // XXX TODO ACM ERROR
                }
                ti.data_type = *data_type;
                ti.shape.reserve(s.shape_size());
                ti.shape.assign(s.shape().begin(), s.shape().end());
                ti.associated_files.reserve(s.associated_files().size());
                for (auto&& af : *s.mutable_associated_files()) {
                    ti.associated_files.emplace_back();
                    auto& new_file = ti.associated_files.back();
                    new_file.name = std::move(*af.mutable_name());
                    new_file.description = std::move(*af.mutable_description());
                    switch (af.label_type()) {
                        case ::viam::service::mlmodel::v1::LABEL_TYPE_TENSOR_VALUE:
                            new_file.label_type = tensor_info::file::k_label_type_tensor_value;
                            break;
                        case ::viam::service::mlmodel::v1::LABEL_TYPE_TENSOR_AXIS:
                            new_file.label_type = tensor_info::file::k_label_type_tensor_axis;
                            break;
                        default:
                            throw -1;  // XXX ACM TODO
                    }
                }
                if (s.has_extra()) {
                    ti.extra = struct_to_map(s.extra());
                }
            }
        };

        unpack_tensor_info(result.inputs, *metadata_pb.mutable_input_info());
        unpack_tensor_info(result.outputs, *metadata_pb.mutable_output_info());
    }
    return result;
}

}  // namespace sdk
}  // namespace viam
