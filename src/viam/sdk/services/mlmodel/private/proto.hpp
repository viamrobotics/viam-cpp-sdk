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

#pragma once

#include <vector>

#include <viam/sdk/services/mlmodel/mlmodel.hpp>

namespace viam {
namespace sdk {
namespace mlmodel_details {

using tensor_storage_types = boost::mpl::transform_view<MLModelService::base_types,
                                                        std::vector<boost::mpl::placeholders::_1>>;

using tensor_storage = std::vector<boost::make_variant_over<tensor_storage_types>::type>;

::grpc::Status pb_value_to_tensor(const MLModelService::tensor_info& tensor_info,
                                  const ::google::protobuf::Value& pb,
                                  tensor_storage* ts,
                                  MLModelService::named_tensor_views* ntvs);

::grpc::Status tensor_to_pb_value(const MLModelService::tensor_views& tensor,
                                  ::google::protobuf::Value* value);

void copy_sdk_tensor_to_api_tensor(const MLModelService::tensor_views& source,
                                   ::viam::service::mlmodel::v1::FlatTensor* target);

MLModelService::tensor_views make_sdk_tensor_from_api_tensor(
    const ::viam::service::mlmodel::v1::FlatTensor& api_tensor, tensor_storage* storage = nullptr);

}  // namespace mlmodel_details
}  // namespace sdk
}  // namespace viam
