#include <viam/sdk/components/base/base.hpp>

#include <google/protobuf/descriptor.h>

#include <viam/api/component/base/v1/base.grpc.pb.h>
#include <viam/api/component/base/v1/base.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/base/client.hpp>
#include <viam/sdk/components/base/server.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

BaseClient::BaseClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Base(std::move(name)),
      stub_(viam::component::base::v1::BaseService::NewStub(channel)),
      channel_(std::move(channel)){};

std::shared_ptr<ResourceServer> BaseRegistration::create_resource_server(
    std::shared_ptr<ResourceManager> manager) {
    return std::make_shared<BaseServer>(manager);
};

std::shared_ptr<Resource> BaseRegistration::create_rpc_client(std::string name,
                                                              std::shared_ptr<grpc::Channel> chan) {
    return std::make_shared<BaseClient>(std::move(name), std::move(chan));
};

std::shared_ptr<ResourceRegistration> Base::resource_registration() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::base::v1::BaseService::service_full_name());
    if (!sd) {
        throw ViamException("Unable to get service descriptor for the base service");
    }
    return std::make_shared<BaseRegistration>(sd);
}

API Base::static_api() {
    return API(RDK, COMPONENT, "base");
}

API Base::dynamic_api() const {
    return static_api();
}

Base::Base(std::string name) : Component(std::move(name)){};

namespace {
bool init() {
    Registry::register_resource(Base::static_api(), Base::resource_registration());
    return true;
};

// NOLINTNEXTLINE
const bool inited = init();
}  // namespace

}  // namespace sdk
}  // namespace viam
