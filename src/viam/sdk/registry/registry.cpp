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

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/components/private/arm_client.hpp>
#include <viam/sdk/components/private/arm_server.hpp>
#include <viam/sdk/components/private/base_client.hpp>
#include <viam/sdk/components/private/base_server.hpp>
#include <viam/sdk/components/private/board_client.hpp>
#include <viam/sdk/components/private/board_server.hpp>
#include <viam/sdk/components/private/camera_client.hpp>
#include <viam/sdk/components/private/camera_server.hpp>
#include <viam/sdk/components/private/encoder_client.hpp>
#include <viam/sdk/components/private/encoder_server.hpp>
#include <viam/sdk/components/private/generic_client.hpp>
#include <viam/sdk/components/private/generic_server.hpp>
#include <viam/sdk/components/private/motor_client.hpp>
#include <viam/sdk/components/private/motor_server.hpp>
#include <viam/sdk/components/private/movement_sensor_client.hpp>
#include <viam/sdk/components/private/movement_sensor_server.hpp>
#include <viam/sdk/components/private/power_sensor_client.hpp>
#include <viam/sdk/components/private/power_sensor_server.hpp>
#include <viam/sdk/components/private/sensor_client.hpp>
#include <viam/sdk/components/private/sensor_server.hpp>
#include <viam/sdk/components/private/servo_client.hpp>
#include <viam/sdk/components/private/servo_server.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/services/private/generic_client.hpp>
#include <viam/sdk/services/private/generic_server.hpp>
#include <viam/sdk/services/private/mlmodel_client.hpp>
#include <viam/sdk/services/private/mlmodel_server.hpp>
#include <viam/sdk/services/private/motion_client.hpp>
#include <viam/sdk/services/private/motion_server.hpp>
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
        const std::string err = "Cannot register API/model pair" + reg_key +
                                " as that pair has already been registered";
        throw Exception(ErrorCondition::k_duplicate_registration, err);
    }

    resources_.emplace(std::move(reg_key), std::move(resource));
}

void Registry::register_resource_server_(
    API api, std::shared_ptr<ResourceServerRegistration> resource_registration) {
    if (server_apis_.find(api) != server_apis_.end()) {
        const std::string err =
            "Cannot register server API" + api.to_string() + " as it has already been registered";
        throw Exception(ErrorCondition::k_duplicate_registration, err);
    }

    server_apis_.emplace(std::move(api), std::move(resource_registration));
}

void Registry::register_resource_client_(
    API api, std::shared_ptr<ResourceClientRegistration> resource_registration) {
    if (client_apis_.find(api) != client_apis_.end()) {
        const std::string err =
            "Cannot register client API" + api.to_string() + " as it has already been registered";
        throw Exception(ErrorCondition::k_duplicate_registration, err);
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
        throw Exception("Unable to get service descriptor");
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
    Registry::register_resource<impl::ArmClient, impl::ArmServer>();
    Registry::register_resource<impl::BaseClient, impl::BaseServer>();
    Registry::register_resource<impl::BoardClient, impl::BoardServer>();
    Registry::register_resource<impl::CameraClient, impl::CameraServer>();
    Registry::register_resource<impl::EncoderClient, impl::EncoderServer>();
    Registry::register_resource<impl::GenericComponentClient, impl::GenericComponentServer>();
    Registry::register_resource<impl::MotorClient, impl::MotorServer>();
    Registry::register_resource<impl::MovementSensorClient, impl::MovementSensorServer>();
    Registry::register_resource<impl::PowerSensorClient, impl::PowerSensorServer>();
    Registry::register_resource<impl::SensorClient, impl::SensorServer>();
    Registry::register_resource<impl::ServoClient, impl::ServoServer>();

    // Register all services
    Registry::register_resource<impl::GenericServiceClient, impl::GenericServiceServer>();
    Registry::register_resource<impl::MLModelServiceClient, impl::MLModelServiceServer>();
    Registry::register_resource<impl::MotionClient, impl::MotionServer>();
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
