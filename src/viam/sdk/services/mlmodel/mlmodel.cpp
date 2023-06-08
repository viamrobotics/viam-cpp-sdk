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

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/services/mlmodel/client.hpp>
#include <viam/sdk/services/mlmodel/server.hpp>

namespace viam {
namespace sdk {

MLModelServiceRegistration::MLModelServiceRegistration(
    const google::protobuf::ServiceDescriptor* service_descriptor)
    : ResourceRegistration(service_descriptor) {}

std::shared_ptr<ResourceServer> MLModelServiceRegistration::create_resource_server(
    std::shared_ptr<ResourceManager> manager) {
    return std::make_shared<MLModelServiceServer>(std::move(manager));
};

std::shared_ptr<Resource> MLModelServiceRegistration::create_rpc_client(
    std::string name, std::shared_ptr<grpc::Channel> channel) {
    return std::make_shared<MLModelServiceClient>(std::move(name), std::move(channel));
};

API MLModelService::static_api() {
    return API(kRDK, kService, "mlmodel");
}

std::shared_ptr<ResourceRegistration> MLModelService::resource_registration() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::service::mlmodel::v1::MLModelService::service_full_name());
    if (!sd) {
        // TODO: Throw viam exception once PR #100 merges.
        throw std::runtime_error("Unable to get service descriptor for the camera service");
    }
    return std::make_shared<MLModelServiceRegistration>(sd);
}

API MLModelService::dynamic_api() const {
    return static_api();
}

boost::optional<MLModelService::tensor_info::data_types>
MLModelService::tensor_info::string_to_data_type(const std::string& str) {
    // TODO: I'm guessing at the string names here, because the scope
    // doesn't actually specify them.
    if (str == "int8") {
        return data_types::k_int8;
    }
    if (str == "uint8") {
        return data_types::k_uint8;
    }
    if (str == "int16") {
        return data_types::k_int16;
    }
    if (str == "uint16") {
        return data_types::k_uint16;
    }
    if (str == "int32") {
        return data_types::k_int32;
    }
    if (str == "uint32") {
        return data_types::k_uint32;
    }
    if (str == "int64") {
        return data_types::k_int64;
    }
    if (str == "uint64") {
        return data_types::k_uint64;
    }
    if (str == "float32") {
        return data_types::k_float32;
    }
    if (str == "float64") {
        return data_types::k_float64;
    }
    return {};
}

const char* MLModelService::tensor_info::data_type_to_string(const data_types data_type) {
    // TODO: I'm guessing at the string names here, because the scope
    // doesn't actually specify them.
    if (data_type == data_types::k_int8) {
        return "int8";
    }
    if (data_type == data_types::k_uint8) {
        return "uint8";
    }
    if (data_type == data_types::k_int16) {
        return "int16";
    }
    if (data_type == data_types::k_uint16) {
        return "uint16";
    }
    if (data_type == data_types::k_int32) {
        return "int32";
    }
    if (data_type == data_types::k_uint32) {
        return "uint32";
    }
    if (data_type == data_types::k_int64) {
        return "int64";
    }
    if (data_type == data_types::k_uint64) {
        return "uint64";
    }
    if (data_type == data_types::k_float32) {
        return "float32";
    }
    if (data_type == data_types::k_float64) {
        return "float64";
    }
    return nullptr;
}

MLModelService::MLModelService(std::string name) : Service(std::move(name)) {}

namespace {
bool init() {
    Registry::register_resource(MLModelService::static_api(),
                                MLModelService::resource_registration());
    return true;
};

// NOLINTNEXTLINE
const bool inited = init();
}  // namespace

}  // namespace sdk
}  // namespace viam
