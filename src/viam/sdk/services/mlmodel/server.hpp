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

#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>


namespace viam {
namespace sdk {

///
/// The `MLModelServiceServer` is used to implement new modular
/// resources implementing the ML Model Service.
///
class MLModelServiceServer : public ResourceServerBase,
                             public ::viam::service::mlmodel::v1::MLModelService::Service {
   public:
    MLModelServiceServer();
    explicit MLModelServiceServer(std::shared_ptr<ResourceManager> manager);

    void register_server(std::shared_ptr<Server> server) override;

    ::grpc::Status Infer(::grpc::ServerContext* context,
                         const ::viam::service::mlmodel::v1::InferRequest* request,
                         ::viam::service::mlmodel::v1::InferResponse* response) override;

    ::grpc::Status Metadata(::grpc::ServerContext* context,
                            const ::viam::service::mlmodel::v1::MetadataRequest* request,
                            ::viam::service::mlmodel::v1::MetadataResponse* response) override;

};

}  // namespace sdk
}  // namespace viam
