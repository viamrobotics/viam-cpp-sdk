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

#include <grpcpp/channel.h>

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

    std::shared_ptr<named_tensor_views> infer(const named_tensor_views& inputs,
                                              const AttributeMap& extra) override;
    struct metadata metadata(const AttributeMap& extra) override;

    // the `extra` param is frequently unnecessary but needs to be supported. Ideally, we'd
    // like to live in a world where implementers of derived classes don't need to go out of
    // their way to support two versions of a method (an `extra` version and a non-`extra`
    // version), and users don't need to pass an unnecessary parameters to all method calls.
    //
    // To do this, we define in the parent resource class a non-virtual version of the methods
    // that calls the virtual method and passes a `nullptr` by default in place of the `extra`
    // param. In order to access these versions of the methods within the client code, however,
    // we need to include these `using` lines.
    using MLModelService::infer;
    using MLModelService::metadata;

   private:
    std::shared_ptr<grpc::Channel> channel_;
    std::unique_ptr<service_type::StubInterface> stub_;
};

}  // namespace sdk
}  // namespace viam
