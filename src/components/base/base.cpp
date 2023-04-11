#include <components/base/base.hpp>

#include <google/protobuf/descriptor.h>

#include <component/base/v1/base.grpc.pb.h>
#include <component/base/v1/base.pb.h>

#include <common/utils.hpp>
#include <components/base/client.hpp>
#include <components/base/server.hpp>
#include <registry/registry.hpp>
#include <resource/resource.hpp>

std::shared_ptr<ResourceServerBase> BaseSubtype::create_resource_server(
    std::shared_ptr<SubtypeService> svc) {
    return std::make_shared<BaseServer>(svc);
};

std::shared_ptr<ResourceBase> BaseSubtype::create_rpc_client(std::string name,
                                                             std::shared_ptr<grpc::Channel> chan) {
    return std::make_shared<BaseClient>(std::move(name), std::move(chan));
};

std::shared_ptr<ResourceSubtype> Base::resource_subtype() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::base::v1::BaseService::service_full_name());
    if (sd == nullptr) {
        throw std::runtime_error("Unable to get service descriptor for the base service");
    }
    return std::make_shared<BaseSubtype>(sd);
}

Subtype Base::subtype() {
    return Subtype(RDK, COMPONENT, "base");
}

namespace {
bool init() {
    Registry::register_subtype(Base::subtype(), Base::resource_subtype());
    return true;
};

bool inited = init();
}  // namespace
