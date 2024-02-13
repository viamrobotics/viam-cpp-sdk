#include <viam/sdk/registry/registry.hpp>

#include <exception>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/log/trivial.hpp>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/struct.pb.h>
#include <grpcpp/channel.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include <viam/sdk/components/base/client.hpp>
#include <viam/sdk/components/base/server.hpp>
#include <viam/sdk/components/board/client.hpp>
#include <viam/sdk/components/board/server.hpp>
#include <viam/sdk/components/camera/client.hpp>
#include <viam/sdk/components/camera/server.hpp>
#include <viam/sdk/components/encoder/client.hpp>
#include <viam/sdk/components/encoder/server.hpp>
#include <viam/sdk/components/generic/client.hpp>
#include <viam/sdk/components/generic/server.hpp>
#include <viam/sdk/components/motor/client.hpp>
#include <viam/sdk/components/motor/server.hpp>
#include <viam/sdk/components/movement_sensor/client.hpp>
#include <viam/sdk/components/movement_sensor/server.hpp>
#include <viam/sdk/components/power_sensor/client.hpp>
#include <viam/sdk/components/power_sensor/server.hpp>
#include <viam/sdk/components/sensor/client.hpp>
#include <viam/sdk/components/sensor/server.hpp>
#include <viam/sdk/components/servo/client.hpp>
#include <viam/sdk/components/servo/server.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/services/generic/client.hpp>
#include <viam/sdk/services/generic/server.hpp>
#include <viam/sdk/services/mlmodel/client.hpp>
#include <viam/sdk/services/mlmodel/server.hpp>
#include <viam/sdk/services/motion/client.hpp>
#include <viam/sdk/services/motion/server.hpp>
#include <viam/sdk/services/service.hpp>

namespace viam {
namespace sdk {

using viam::robot::v1::Status;

ResourceServerRegistration::~ResourceServerRegistration() = default;
ResourceClientRegistration::~ResourceClientRegistration() = default;

const API& ModelRegistration::api() const {
    return api_;
};
const Model& ModelRegistration::model() const {
    return model_;
};

void Registry::register_model(std::shared_ptr<const ModelRegistration> resource) {
    std::string reg_key = resource->api().to_string() + "/" + resource->model().to_string();
    if (resources_.find(reg_key) != resources_.end()) {
        const std::string err = "Cannot add resource with name " + reg_key + "as it already exists";
        throw std::runtime_error(err);
    }

    resources_.emplace(std::move(reg_key), std::move(resource));
}

void Registry::register_resource_server_(
    API api, std::shared_ptr<ResourceServerRegistration> resource_registration) {
    if (server_apis_.find(api) != server_apis_.end()) {
        throw std::runtime_error("Cannot add api " + api.to_string() + " as it already exists");
    }

    server_apis_.emplace(std::move(api), std::move(resource_registration));
}

void Registry::register_resource_client_(
    API api, std::shared_ptr<ResourceClientRegistration> resource_registration) {
    if (client_apis_.find(api) != client_apis_.end()) {
        throw std::runtime_error("Cannot add api " + api.to_string() + " as it already exists");
    }

    client_apis_.emplace(std::move(api), std::move(resource_registration));
}

std::shared_ptr<const ModelRegistration> Registry::lookup_model_inlock_(
    const std::string& name, const std::lock_guard<std::mutex>&) {
    if (resources_.find(name) == resources_.end()) {
        return nullptr;
    }

    return resources_.at(name);
}

std::shared_ptr<const ModelRegistration> Registry::lookup_model(const std::string& name) {
    const std::lock_guard<std::mutex> lock(lock_);
    return lookup_model_inlock_(name, lock);
}

std::shared_ptr<const ModelRegistration> Registry::lookup_model(const API& api,
                                                                const Model& model) {
    const std::lock_guard<std::mutex> lock(lock_);
    const std::string name = api.to_string() + "/" + model.to_string();
    return lookup_model_inlock_(name, lock);
}

std::shared_ptr<const ResourceServerRegistration> Registry::lookup_resource_server(const API& api) {
    const std::lock_guard<std::mutex> lock(lock_);
    if (server_apis_.find(api) == server_apis_.end()) {
        return nullptr;
    }

    return server_apis_.at(api);
}

std::shared_ptr<const ResourceClientRegistration> Registry::lookup_resource_client(const API& api) {
    const std::lock_guard<std::mutex> lock(lock_);
    if (client_apis_.find(api) == client_apis_.end()) {
        return nullptr;
    }

    return client_apis_.at(api);
}

const google::protobuf::ServiceDescriptor* Registry::get_service_descriptor_(
    const char* service_full_name) {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd = p->FindServiceByName(service_full_name);
    if (!sd) {
        throw std::runtime_error("Unable to get service descriptor");
    }
    return sd;
}

const std::unordered_map<API, std::shared_ptr<const ResourceServerRegistration>>&
Registry::registered_resource_servers() {
    return server_apis_;
}

const std::unordered_map<std::string, std::shared_ptr<const ModelRegistration>>&
Registry::registered_models() {
    return resources_;
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
Status ModelRegistration::create_status(const std::shared_ptr<Resource>& resource) const {
    Status status;
    *status.mutable_name() = resource->get_resource_name(resource->name());
    *status.mutable_status() = google::protobuf::Struct();
    return status;
}

const google::protobuf::ServiceDescriptor* ResourceServerRegistration::service_descriptor() const {
    return service_descriptor_;
}

void register_resources() {
    // Register all components
    Registry::register_resource<BaseClient, BaseServer>();
    Registry::register_resource<BoardClient, BoardServer>();
    Registry::register_resource<CameraClient, CameraServer>();
    Registry::register_resource<EncoderClient, EncoderServer>();
    Registry::register_resource<GenericComponentClient, GenericComponentServer>();
    Registry::register_resource<MotorClient, MotorServer>();
    Registry::register_resource<MovementSensorClient, MovementSensorServer>();
    Registry::register_resource<PowerSensorClient, PowerSensorServer>();
    Registry::register_resource<SensorClient, SensorServer>();
    Registry::register_resource<ServoClient, ServoServer>();

    // Register all services
    Registry::register_resource<GenericServiceClient, GenericServiceServer>();
    Registry::register_resource<MLModelServiceClient, MLModelServiceServer>();
    Registry::register_resource<MotionClient, MotionServer>();
}

void Registry::initialize() {
    const std::lock_guard<std::mutex> lock(lock_);
    if (initialized_) {
        BOOST_LOG_TRIVIAL(warning)
            << "Attempted to initialize the Registry but it was already initialized.";
        return;
    }
    initialized_ = true;
    register_resources();
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
}

std::unordered_map<std::string, std::shared_ptr<const ModelRegistration>> Registry::resources_;
std::unordered_map<API, std::shared_ptr<const ResourceClientRegistration>> Registry::client_apis_;
std::unordered_map<API, std::shared_ptr<const ResourceServerRegistration>> Registry::server_apis_;
std::mutex Registry::lock_;
bool Registry::initialized_{false};

}  // namespace sdk
}  // namespace viam
