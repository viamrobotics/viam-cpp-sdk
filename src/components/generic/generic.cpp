#include <components/generic/generic.hpp>

#include <stdexcept>

#include <google/protobuf/descriptor.h>

#include <component/generic/v1/generic.grpc.pb.h>

#include <common/utils.hpp>
#include <components/generic/client.hpp>
#include <components/generic/server.hpp>
#include <registry/registry.hpp>
#include <resource/resource.hpp>

namespace viam {
namespace cppsdk {

std::shared_ptr<ResourceServerBase> GenericSubtype::create_resource_server(
    std::shared_ptr<SubtypeService> svc) {
    return std::make_shared<GenericServer>(svc);
};

std::shared_ptr<ResourceBase> GenericSubtype::create_rpc_client(
    std::string name, std::shared_ptr<grpc::Channel> chan) {
    return std::make_shared<GenericClient>(std::move(name), std::move(chan));
};

std::shared_ptr<ResourceSubtype> Generic::resource_subtype() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::generic::v1::GenericService::service_full_name());
    if (sd == nullptr) {
        throw std::runtime_error("Unable to get service descriptor for the generic service");
    }
    return std::make_shared<GenericSubtype>(sd);
}

Subtype Generic::subtype() {
    return Subtype(RDK, COMPONENT, "generic");
}

namespace {
bool init() {
    Registry::register_subtype(Generic::subtype(), Generic::resource_subtype());
    return true;
};

bool inited = init();
}  // namespace

}  // namespace cppsdk
}  // namespace viam
