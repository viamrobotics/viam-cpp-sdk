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

#include <viam/api/service/mlmodel/v1/mlmodel.grpc.pb.h>

#include <viam/sdk/services/mlmodel/mlmodel.hpp>

namespace viam {
namespace sdk {

///
/// An `MLModelServiceClient` provides client-side access to a
/// remotely served ML Model Service. Use this class to communicate
/// with MLModelService instances running elsewhere.
///
class MLModelServiceClient : public MLModelService {
public:
    using service_type = viam::service::mlmodel::v1::MLModelService;

    MLModelServiceClient(std::string name, std::shared_ptr<grpc::Channel> channel);

    void infer() override;
    struct metadata metadata() override;

protected:
    MLModelServiceClient(std::string name, std::unique_ptr<service_type::StubInterface> stub);

private:
    std::shared_ptr<grpc::Channel> channel_;
    std::unique_ptr<service_type::StubInterface> stub_;
};

}  // namespace sdk
}  // namespace viam
