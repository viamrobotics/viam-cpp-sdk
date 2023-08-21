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

#include <viam/sdk/tests/mocks/mlmodel_mocks.hpp>

#include <viam/sdk/common/proto_type.hpp>

namespace viam {
namespace sdktests {

MockMLModelService& MockMLModelService::set_infer_handler(infer_handler handler) {
    infer_handler_ = std::move(handler);
    return *this;
}

std::shared_ptr<sdk::MLModelService::named_tensor_views> MockMLModelService::infer(
    const named_tensor_views& inputs) {
    return infer(inputs, nullptr);
}

std::shared_ptr<sdk::MLModelService::named_tensor_views> MockMLModelService::infer(
    const named_tensor_views& inputs, const sdk::AttributeMap& extra) {
    return infer_handler_(inputs);
}

MockMLModelService& MockMLModelService::set_metadata(struct metadata metadata) {
    metadata_ = std::move(metadata);
    return *this;
}

struct sdk::MLModelService::metadata MockMLModelService::metadata(const sdk::AttributeMap& extra) {
    return metadata_;
}

struct sdk::MLModelService::metadata MockMLModelService::metadata() {
    return metadata(nullptr);
}

}  // namespace sdktests
}  // namespace viam
