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

#include <viam/sdk/services/mlmodel/mlmodel.hpp>

#include <viam/sdk/services/mlmodel/client.hpp>
#include <viam/sdk/services/mlmodel/server.hpp>

namespace viam {
namespace sdk {

MLModelServiceSubtype::MLModelServiceSubtype(
    const google::protobuf::ServiceDescriptor* service_descriptor)
    : ResourceSubtype(service_descriptor) {}

std::shared_ptr<ResourceServer> MLModelServiceSubtype::create_resource_server(
    std::shared_ptr<ResourceManager> manager) {
    return std::make_shared<MLModelServiceServer>(std::move(manager));
};

std::shared_ptr<Resource> MLModelServiceSubtype::create_rpc_client(
    std::string name, std::shared_ptr<grpc::Channel> channel) {
    return std::make_shared<MLModelServiceClient>(std::move(name), std::move(channel));
};

MLModelService::MLModelService(std::string name) : Service(std::move(name)) {}

std::shared_ptr<ResourceSubtype> MLModelService::resource_subtype() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::service::mlmodel::v1::MLModelService::service_full_name());
    if (!sd) {
        throw std::runtime_error("Unable to get service descriptor for the camera service");
    }
    return std::make_shared<MLModelServiceSubtype>(sd);
}

}  // namespace sdk
}  // namespace viam
