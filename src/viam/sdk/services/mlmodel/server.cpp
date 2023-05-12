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

#include <strstream>

#include <boost/variant/get.hpp>

#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/services/mlmodel/mlmodel.hpp>

namespace viam {
namespace sdk {

namespace {

::grpc::Status to_infer_request(const MLModelService::tensor_info& tensor_info,
                                const ::google::protobuf::Value& pb,
                                MLModelService::infer_request* infer_request) {
    return ::grpc::Status();
}

::grpc::Status from_infer_response(
    const MLModelService::tensor_info& tensor_info,
    const MLModelService::tensor_views& tensor,
    decltype(std::declval<::google::protobuf::Struct>().mutable_fields()) fields) {
    auto emplace_result = fields->try_emplace(tensor_info.name);
    auto contained = boost::get<MLModelService::tensor_view<float>::type>(&tensor);

    // TODO: Write the iterative version, but for now I need no more than
    // depth 3 double array to make a detector work.
    if (contained->shape().size() == 1) {
        auto lv = std::make_unique<::google::protobuf::ListValue>();
        auto* lv_mutable_vals = lv->mutable_values();
        for (size_t i = 0; i != contained->shape()[0]; ++i) {
            auto new_value = std::make_unique<::google::protobuf::Value>();
            new_value->set_number_value((*contained)(i));
            lv_mutable_vals->AddAllocated(new_value.release());
        }
        emplace_result.first->second.set_allocated_list_value(lv.release());
    } else if (contained->shape().size() == 2) {
        auto lv1 = std::make_unique<::google::protobuf::ListValue>();
        auto* lv1_mutable_vals = lv1->mutable_values();
        for (size_t i = 0; i != contained->shape()[0]; ++i) {
            auto lv2 = std::make_unique<::google::protobuf::ListValue>();
            auto* lv2_mutable_vals = lv2->mutable_values();
            for (size_t j = 0; j != contained->shape()[1]; ++j) {
                auto new_value = std::make_unique<::google::protobuf::Value>();
                new_value->set_number_value((*contained)(i, j));
                lv2_mutable_vals->AddAllocated(new_value.release());
            }
            auto wrap_value = std::make_unique<::google::protobuf::Value>();
            wrap_value->set_allocated_list_value(lv2.release());
            lv1_mutable_vals->AddAllocated(wrap_value.release());
        }
        emplace_result.first->second.set_allocated_list_value(lv1.release());
    } else if (contained->shape().size() == 3) {
        auto lv1 = std::make_unique<::google::protobuf::ListValue>();
        auto* lv1_mutable_vals = lv1->mutable_values();
        for (size_t i = 0; i != contained->shape()[0]; ++i) {
            auto lv2 = std::make_unique<::google::protobuf::ListValue>();
            auto* lv2_mutable_vals = lv2->mutable_values();
            for (size_t j = 0; j != contained->shape()[1]; ++j) {
                auto lv3 = std::make_unique<::google::protobuf::ListValue>();
                auto* lv3_mutable_vals = lv3->mutable_values();
                for (size_t k = 0; k != contained->shape()[2]; ++k) {
                    auto new_value = std::make_unique<::google::protobuf::Value>();
                    new_value->set_number_value((*contained)(i, j, k));
                    lv3_mutable_vals->AddAllocated(new_value.release());
                }
                auto wrap_value = std::make_unique<::google::protobuf::Value>();
                wrap_value->set_allocated_list_value(lv3.release());
                lv2_mutable_vals->AddAllocated(wrap_value.release());
            }
            auto wrap_value = std::make_unique<::google::protobuf::Value>();
            wrap_value->set_allocated_list_value(lv2.release());
            lv1_mutable_vals->AddAllocated(wrap_value.release());
        }
        emplace_result.first->second.set_allocated_list_value(lv1.release());
    }

    return ::grpc::Status();
}

}  // namespace

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

    // TODO: Caching for metadata. It is a complex structure with
    // dynamic allocations, so ideally we don't need to make a copy
    // for every inference. Unfortunately, we can't make the
    // `metadata` method in the base class reutrn a `const&` because
    // on the client side it needs to synthesize a new object to
    // return on each call.
    const auto md = mlms->metadata();

    if (!request->has_input_data()) {
        return {::grpc::StatusCode::INVALID_ARGUMENT, "Called [Infer] with no input data"};
    }
    const auto& pb_input_data_fields = request->input_data().fields();

    MLModelService::infer_request infer_request;

    for (const auto& input : md.inputs) {
        const auto where = pb_input_data_fields.find(input.name);
        if (where == pb_input_data_fields.end()) {
            std::strstream message;
            message << "Call to [Infer] missing metadata-specified input tensor '" << input.name
                    << "'";
            return {::grpc::StatusCode::INVALID_ARGUMENT, message.str()};
        }

        const auto status = to_infer_request(input, where->second, &infer_request);

        if (!status.ok())
            return status;
    }

    // TODO: Should we handle exceptions here? Or is it ok to let them
    // bubble up and have a higher layer deal with it?
    const auto infer_result = mlms->infer(infer_request);

    auto& pb_output_data_fields = *(response->mutable_output_data()->mutable_fields());
    for (const auto& output : md.outputs) {
        const auto where = infer_result.second.find(output.name);
        if (where == infer_result.second.end()) {
            std::strstream message;
            message << "MLModelService::infer results missing metadata-mandated output tensor'"
                    << output.name << "' for [Infer] invocation";
            return {::grpc::StatusCode::INTERNAL, message.str()};
        }

        const auto status = from_infer_response(output, where->second, &pb_output_data_fields);

        if (!status.ok())
            return status;
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
    auto result = mlms->metadata();

    auto& metadata_pb = *response->mutable_metadata();
    *metadata_pb.mutable_name() = std::move(result.name);
    *metadata_pb.mutable_type() = std::move(result.type);
    *metadata_pb.mutable_description() = std::move(result.description);

    const auto pack_tensor_info = [](auto& target,
                                     const std::vector<MLModelService::tensor_info>& source) {
        target.Reserve(source.size());
        for (auto&& s : source) {
            auto& new_entry = *target.Add();
            *new_entry.mutable_name() = std::move(s.name);
            *new_entry.mutable_description() = std::move(s.description);
            *new_entry.mutable_data_type() = std::move(s.data_type);
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
                    case MLModelService::tensor_info::file::k_type_tensor_value:
                        new_af.set_label_type(
                            ::viam::service::mlmodel::v1::LABEL_TYPE_TENSOR_VALUE);
                        break;
                    case MLModelService::tensor_info::file::k_type_tensor_axis:
                        new_af.set_label_type(::viam::service::mlmodel::v1::LABEL_TYPE_TENSOR_AXIS);
                        break;
                    default:
                        break;
                        // XXX ACM TODO
                }
            }
            // XXX ACM TODO: `extra` field
        }
    };

    pack_tensor_info(*metadata_pb.mutable_input_info(), result.inputs);
    pack_tensor_info(*metadata_pb.mutable_output_info(), result.outputs);

    return ::grpc::Status();
}

}  // namespace sdk
}  // namespace viam
