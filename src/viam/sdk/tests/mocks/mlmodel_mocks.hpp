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

#include <functional>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/services/mlmodel/mlmodel.hpp>

namespace viam {
namespace sdktests {

class MockMLModelService : public sdk::MLModelService {
   public:
    explicit MockMLModelService(std::string name = "mock_mlmodel")
        : MLModelService(std::move(name)) {}

    using infer_handler =
        std::function<std::shared_ptr<named_tensor_views>(const named_tensor_views&)>;
    MockMLModelService& set_infer_handler(infer_handler handler);
    std::shared_ptr<named_tensor_views> infer(const named_tensor_views& inputs) override;
    std::shared_ptr<named_tensor_views> infer(const named_tensor_views& inputs,
                                              const sdk::AttributeMap& extra) override;

    MockMLModelService& set_metadata(struct metadata metadata);
    struct metadata metadata() override;
    struct metadata metadata(const sdk::AttributeMap& extra) override;

   private:
    infer_handler infer_handler_;
    struct metadata metadata_;
};

}  // namespace sdktests
}  // namespace viam
