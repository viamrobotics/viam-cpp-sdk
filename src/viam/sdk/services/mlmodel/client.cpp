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

namespace viam {
namespace sdk {

MLModelServiceClient::MLModelServiceClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : MLModelServiceClient(std::move(name), service_type::NewStub(channel)) {
    channel_ = std::move(channel);
}

MLModelServiceClient::MLModelServiceClient(std::string name, std::unique_ptr<service_type::StubInterface> stub)
    : MLModelService(name),
      stub_(std::move(stub)) {
}

void MLModelServiceClient::infer() {
}

struct MLModelService::metadata MLModelServiceClient::metadata() {
    return {};
}

}  // namespace sdk
}  // namespace viam
