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

#include <grpcpp/channel.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/services/mlmodel/private/proto.hpp>

namespace viam {
namespace sdk {

MLModelServiceClient::MLModelServiceClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : MLModelService(std::move(name)),
      channel_(std::move(channel)),
      stub_(service_type::NewStub(channel_)) {}

std::shared_ptr<MLModelService::named_tensor_views> MLModelServiceClient::infer(
    const named_tensor_views& inputs, const AttributeMap& extra) {
    namespace pb = ::google::protobuf;
    namespace mlpb = ::viam::service::mlmodel::v1;

    auto arena = std::make_unique<pb::Arena>();
    auto* const req = pb::Arena::CreateMessage<mlpb::InferRequest>(arena.get());

    req->set_name(this->name());
    *req->mutable_extra() = map_to_struct(extra);
    auto* const resp = pb::Arena::CreateMessage<mlpb::InferResponse>(arena.get());
    grpc::ClientContext ctx;
    set_client_ctx_authority(ctx);

    struct arena_and_views {
        // NOTE: It is not necessary to capture the `resp` pointer
        // here, since the lifetime of that object is subsumed by
        // the arena.
        std::unique_ptr<pb::Arena> arena;
        MLModelService::named_tensor_views views;
    };
    auto aav = std::make_shared<arena_and_views>();
    aav->arena = std::move(arena);

    auto& input_tensors = *req->mutable_input_tensors()->mutable_tensors();
    for (const auto& kv : inputs) {
        auto& emplaced = input_tensors[kv.first];
        mlmodel_details::copy_sdk_tensor_to_api_tensor(kv.second, &emplaced);
    }

    const auto result = stub_->Infer(&ctx, *req, resp);
    if (!result.ok()) {
        throw std::runtime_error(result.error_message());
    }

    for (const auto& kv : resp->output_tensors().tensors()) {
        // NOTE: We don't need to pass in tensor storage here,
        // because the backing store for the views is the Arena we
        // moved into our result above.
        auto tensor = mlmodel_details::make_sdk_tensor_from_api_tensor(kv.second);
        aav->views.emplace(kv.first, std::move(tensor));
    }
    auto* const tsav_views = &aav->views;
    return {std::move(aav), tsav_views};
}

struct MLModelService::metadata MLModelServiceClient::metadata(const AttributeMap& extra) {
    // Encode metadata args into a `MetadataRequest`
    viam::service::mlmodel::v1::MetadataRequest req;
    *req.mutable_name() = name();
    *req.mutable_extra() = map_to_struct(extra);

    // Invoke the stub
    grpc::ClientContext ctx;
    set_client_ctx_authority(ctx);
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
                    std::ostringstream message;
                    message << "Failed to deserialize returned Metadata.TensorInfo.data_type field "
                               "with value `"
                            << s.data_type() << "` to one of the known tensor data types";
                    // TODO: Throw viam exception once PR #100 merges.
                    throw std::runtime_error(message.str());
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
                        case ::viam::service::mlmodel::v1::LABEL_TYPE_TENSOR_VALUE: {
                            new_file.label_type = tensor_info::file::k_label_type_tensor_value;
                            break;
                        }
                        case ::viam::service::mlmodel::v1::LABEL_TYPE_TENSOR_AXIS: {
                            new_file.label_type = tensor_info::file::k_label_type_tensor_axis;
                            break;
                        }
                        default: {
                            std::ostringstream message;
                            message << "Failed to deserialize returned "
                                       "Metadata.TensorInfo.File.label_type field with value `"
                                    << af.label_type() << "` to one of the known label types";
                            // TODO: Throw viam exception once PR #100 merges.
                            throw std::runtime_error(message.str());
                        }
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
