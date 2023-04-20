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

#include <viam/sdk/services/mlmodel/mlmodel.hpp>
#include <rpc/server.hpp>

namespace viam {
namespace sdk {

MLModelServiceServer::MLModelServiceServer() : MLModelServiceServer(std::make_shared<SubtypeService>()) {}

MLModelServiceServer::MLModelServiceServer(std::shared_ptr<SubtypeService> subtype_service) :
    subtype_service_(std::move(subtype_service)) {}

void MLModelServiceServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

const std::shared_ptr<SubtypeService>& MLModelServiceServer::get_sub_svc() {
    return subtype_service_;
}

::grpc::Status MLModelServiceServer::Infer(
    ::grpc::ServerContext* context,
    const ::viam::service::mlmodel::v1::InferRequest* request,
    ::viam::service::mlmodel::v1::InferResponse* response) {

    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Infer] without a request");
    };

    std::shared_ptr<ResourceBase> rb = get_sub_svc()->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<MLModelService> mlms = std::dynamic_pointer_cast<MLModelService>(rb);
    mlms->infer();

    //*response->mutable_result() = map_to_struct(result);

    return ::grpc::Status();

}

::grpc::Status MLModelServiceServer::Metadata(
    ::grpc::ServerContext* context,
    const ::viam::service::mlmodel::v1::MetadataRequest* request,
    ::viam::service::mlmodel::v1::MetadataResponse* response) {

    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Metadata] without a request");
    };

    std::shared_ptr<ResourceBase> rb = get_sub_svc()->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<MLModelService> mlms = std::dynamic_pointer_cast<MLModelService>(rb);
    auto result = mlms->metadata();

    //*response->mutable_result() = map_to_struct(result);

    return ::grpc::Status();
}

}  // namespace sdk
}  // namespace viam
