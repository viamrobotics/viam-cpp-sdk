#include <viam/sdk/components/generic/generic.hpp>

#include <stdexcept>

#include <google/protobuf/descriptor.h>

#include <viam/api/component/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/generic/client.hpp>
#include <viam/sdk/components/generic/server.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

GenericRegistration::GenericRegistration(
    const google::protobuf::ServiceDescriptor* service_descriptor)
    : ResourceRegistration(service_descriptor){};

std::shared_ptr<ResourceServer> GenericRegistration::create_resource_server(
    std::shared_ptr<ResourceManager> manager) {
    return std::make_shared<GenericServer>(manager);
};

std::shared_ptr<Resource> GenericRegistration::create_rpc_client(
    std::string name, std::shared_ptr<grpc::Channel> chan) {
    return std::make_shared<GenericClient>(std::move(name), std::move(chan));
};

std::shared_ptr<ResourceRegistration> Generic::resource_registration() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::generic::v1::GenericService::service_full_name());
    if (!sd) {
        throw std::runtime_error("Unable to get service descriptor for the generic service");
    }
    return std::make_shared<GenericRegistration>(sd);
}

API Generic::static_api() {
    return {kRDK, kComponent, "generic"};
}

API Generic::dynamic_api() const {
    return static_api();
}

Generic::Generic(std::string name) : Component(std::move(name)){};

namespace {
bool init() {
    Registry::register_resource(Generic::static_api(), Generic::resource_registration());
    return true;
};

// NOLINTNEXTLINE
const bool inited = init();
}  // namespace

}  // namespace sdk
}  // namespace viam
