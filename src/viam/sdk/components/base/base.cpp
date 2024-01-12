#include <viam/sdk/components/base/base.hpp>

#include <google/protobuf/descriptor.h>

#include <viam/api/component/base/v1/base.grpc.pb.h>
#include <viam/api/component/base/v1/base.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/base/client.hpp>
#include <viam/sdk/components/base/server.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

BaseRegistration::BaseRegistration(const google::protobuf::ServiceDescriptor* service_descriptor)
    : ResourceRegistration(service_descriptor){};

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
        throw std::runtime_error("Unable to get service descriptor for the base service");
    }
    return std::make_shared<BaseRegistration>(sd);
}

API Base::api() const {
    return API::for_t<Base>();
}

Base::properties Base::properties::from_proto(
    const component::base::v1::GetPropertiesResponse& proto) {
    return {
        proto.width_meters(), proto.turning_radius_meters(), proto.wheel_circumference_meters()};
}

std::ostream& operator<<(std::ostream& os, const Base::properties& v) {
    os << "{ turning_radius_meters: " << v.turning_radius_meters
       << ", wheel_circumference_meters: " << v.wheel_circumference_meters
       << ", width_meters: " << v.width_meters << " }";
    return os;
}

bool operator==(const Base::properties& lhs, const Base::properties& rhs) {
    return lhs.wheel_circumference_meters == rhs.wheel_circumference_meters &&
           lhs.width_meters == rhs.width_meters &&
           lhs.turning_radius_meters == rhs.turning_radius_meters;
}

Base::Base(std::string name) : Component(std::move(name)){};

namespace {
bool init() {
    Registry::register_resource(API::for_t<Base>(), Base::resource_registration());
    return true;
};

// NOLINTNEXTLINE
const bool inited = init();
}  // namespace

}  // namespace sdk
}  // namespace viam
