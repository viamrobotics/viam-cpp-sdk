#include <viam/sdk/components/servo/servo.hpp>

#include <viam/api/component/servo/v1/servo.grpc.pb.h>
#include <viam/api/component/servo/v1/servo.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/servo/client.hpp>
#include <viam/sdk/components/servo/server.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

std::shared_ptr<ResourceServer> ServoRegistration::create_resource_server(
    std::shared_ptr<ResourceManager> manager, Server& server) {
    auto ss = std::make_shared<ServoServer>(std::move(manager));
    server.register_service(ss.get());
    return ss;
};

std::shared_ptr<Resource> ServoRegistration::create_rpc_client(
    std::string name, std::shared_ptr<grpc::Channel> chan) {
    return std::make_shared<ServoClient>(std::move(name), std::move(chan));
};
ServoRegistration::ServoRegistration(const google::protobuf::ServiceDescriptor* service_descriptor)
    : ResourceRegistration(service_descriptor){};

std::shared_ptr<ResourceRegistration> Servo::resource_registration() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::servo::v1::ServoService::service_full_name());
    if (!sd) {
        throw std::runtime_error("Unable to get service descriptor for the servo service");
    }
    return std::make_shared<ServoRegistration>(sd);
}

API Servo::api() const {
    return API::get<Servo>();
}

API API::traits<Servo>::api() {
    return {kRDK, kComponent, "servo"};
}

Servo::position Servo::from_proto(viam::component::servo::v1::GetPositionResponse proto) {
    return proto.position_deg();
}

namespace {
bool init() {
    Registry::register_resource(API::get<Servo>(), Servo::resource_registration());
    return true;
};

// NOLINTNEXTLINE
const bool inited = init();
}  // namespace

}  // namespace sdk
}  // namespace viam
