#include <viam/sdk/services/generic/generic.hpp>

#include <stdexcept>

#include <google/protobuf/descriptor.h>

#include <viam/api/service/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/services/generic/client.hpp>
#include <viam/sdk/services/generic/server.hpp>

namespace viam {
namespace sdk {

GenericServiceRegistration::GenericServiceRegistration(
    const google::protobuf::ServiceDescriptor* service_descriptor)
    : ResourceRegistration(service_descriptor){};

std::shared_ptr<ResourceServer> GenericServiceRegistration::create_resource_server(
    std::shared_ptr<ResourceManager> manager, Server& server) {
    auto gs = std::make_shared<GenericServiceServer>(std::move(manager));
    server.register_service(gs.get());
    return gs;
};

std::shared_ptr<Resource> GenericServiceRegistration::create_rpc_client(
    std::string name, std::shared_ptr<grpc::Channel> chan) {
    return std::make_shared<GenericServiceClient>(std::move(name), std::move(chan));
};

std::shared_ptr<ResourceRegistration> GenericService::resource_registration() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::generic::v1::GenericService::service_full_name());
    if (!sd) {
        throw std::runtime_error("Unable to get service descriptor for the generic service");
    }
    return std::make_shared<GenericServiceRegistration>(sd);
}

API GenericService::api() const {
    return API::get<GenericService>();
}

API API::traits<GenericService>::api() {
    return {kRDK, kService, "generic"};
}

GenericService::GenericService(std::string name) : Service(std::move(name)){};

namespace {
bool init() {
    Registry::register_resource(API::get<GenericService>(),
                                GenericService::resource_registration());
    return true;
};

// NOLINTNEXTLINE
const bool inited = init();
}  // namespace

}  // namespace sdk
}  // namespace viam
