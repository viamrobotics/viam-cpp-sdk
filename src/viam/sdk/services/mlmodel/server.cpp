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

#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/services/mlmodel/mlmodel.hpp>
#include <viam/sdk/services/mlmodel/private/proto.hpp>

namespace viam {
namespace sdk {

MLModelServiceServer::MLModelServiceServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status MLModelServiceServer::Infer(
    ::grpc::ServerContext* context,
    const ::viam::service::mlmodel::v1::InferRequest* request,
    ::viam::service::mlmodel::v1::InferResponse* response) noexcept {
    return make_service_helper<MLModelService>(
        "MLModelServiceServer::Infer", this, request)([&](auto& helper, auto& mlms) {
        if (!request->has_input_tensors()) {
            return helper.fail(::grpc::INVALID_ARGUMENT, "Called with no input tensors");
        }

        const auto md = mlms->metadata({});
        MLModelService::named_tensor_views inputs;
        for (const auto& input : md.inputs) {
            const auto where = request->input_tensors().tensors().find(input.name);
            if (where == request->input_tensors().tensors().end()) {
                // Ignore any inputs for which we don't have metadata, since
                // we can't validate the type info.
                //
                // TODO: Should this be an error? For now we just don't decode
                // those tensors.
                continue;
            }
            auto tensor = mlmodel_details::make_sdk_tensor_from_api_tensor(where->second);
            const auto tensor_type = MLModelService::tensor_info::tensor_views_to_data_type(tensor);
            if (tensor_type != input.data_type) {
                std::ostringstream message;
                using ut = std::underlying_type<MLModelService::tensor_info::data_types>::type;
                message << "Tensor input `" << input.name << "` was the wrong type; expected type "
                        << static_cast<ut>(input.data_type) << " but got type "
                        << static_cast<ut>(tensor_type);
                return helper.fail(::grpc::INVALID_ARGUMENT, message.str().c_str());
            }
            inputs.emplace(std::move(input.name), std::move(tensor));
        }

        const auto outputs = mlms->infer(inputs, helper.getExtra());

        auto* const output_tensors = response->mutable_output_tensors()->mutable_tensors();
        for (const auto& kv : *outputs) {
            auto& emplaced = (*output_tensors)[kv.first];
            mlmodel_details::copy_sdk_tensor_to_api_tensor(kv.second, &emplaced);
        }

        return ::grpc::Status();
    });
}

::grpc::Status MLModelServiceServer::Metadata(
    ::grpc::ServerContext* context,
    const ::viam::service::mlmodel::v1::MetadataRequest* request,
    ::viam::service::mlmodel::v1::MetadataResponse* response) noexcept {
    return make_service_helper<MLModelService>(
        "MLModelServiceServer::Metadata", this, request)([&](auto& helper, auto& mlms) {
        auto md = mlms->metadata(helper.getExtra());

        auto& metadata_pb = *response->mutable_metadata();
        *metadata_pb.mutable_name() = std::move(md.name);
        *metadata_pb.mutable_type() = std::move(md.type);
        *metadata_pb.mutable_description() = std::move(md.description);

        const auto pack_tensor_info = [&helper](
                                          auto& target,
                                          const std::vector<MLModelService::tensor_info>& source) {
            target.Reserve(source.size());
            for (auto&& s : source) {
                auto& new_entry = *target.Add();
                *new_entry.mutable_name() = std::move(s.name);
                *new_entry.mutable_description() = std::move(s.description);

                const auto* string_for_data_type =
                    MLModelService::tensor_info::data_type_to_string(s.data_type);
                if (!string_for_data_type) {
                    std::ostringstream message;
                    message
                        << "Served MLModelService returned an unknown data type with value `"
                        << static_cast<
                               std::underlying_type<MLModelService::tensor_info::data_types>::type>(
                               s.data_type)
                        << "` in its metadata";
                    return helper.fail(grpc::INTERNAL, message.str().c_str());
                }
                new_entry.set_data_type(string_for_data_type);
                auto& shape = *new_entry.mutable_shape();
                // This would be nicer as `Reserve/Assign`, but older
                // protubuf lacks Assign. The implementation of `Assign`
                // is just `Clear/Add` though, so do that instead.
                shape.Clear();
                shape.Reserve(s.shape.size());
                shape.Add(s.shape.begin(), s.shape.end());
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
                            new_af.set_label_type(
                                ::viam::service::mlmodel::v1::LABEL_TYPE_TENSOR_AXIS);
                            break;
                        default:
                            // In practice this shouldn't really happen
                            // since we shouldn't see an MLMS instance
                            // that we are serving return metadata
                            // containing values not contained in the
                            // enumeration. If it does, we just map it to
                            // unspecified - the client is likely to
                            // interpret it as an error.
                            new_af.set_label_type(
                                ::viam::service::mlmodel::v1::LABEL_TYPE_UNSPECIFIED);
                            break;
                    }
                }
                if (s.extra) {
                    *new_entry.mutable_extra() = map_to_struct(s.extra);
                }
            }
            return ::grpc::Status();
        };

        auto status = pack_tensor_info(*metadata_pb.mutable_input_info(), md.inputs);
        if (!status.ok()) {
            return status;
        }

        return pack_tensor_info(*metadata_pb.mutable_output_info(), md.outputs);
    });
}

}  // namespace sdk
}  // namespace viam
