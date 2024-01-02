#include <viam/sdk/components/servo/server.hpp>

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
        std::shared_ptr<ResourceManager> manager) {
        return std::make_shared<ServoServer>(manager);
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

    API Servo::static_api() {
      return {kRDK, kComponent, "servo"};
    }

    API Servo::dynamic_api() const {
      return static_api();
    }

    Servo::position Servo::from_proto(viam::component::motor::v1::GetPositionResponse proto) {
      return proto.position();
    }

    namespace {
      bool init() {
        Registry::register_resource(Servo::static_api(), Servo::resource_registration());
        return true;
      };

      // NOLINTNEXTLINE
      const bool inited = init();
    }

  }
}