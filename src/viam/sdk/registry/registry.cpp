#include <viam/sdk/registry/registry.hpp>

#include <exception>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/log/trivial.hpp>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/struct.pb.h>
#include <grpcpp/channel.h>

#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/components/component.hpp>
#include <viam/sdk/components/generic/generic.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_type.hpp>
#include <viam/sdk/services/service.hpp>

namespace viam {
namespace sdk {

using viam::robot::v1::Status;

ResourceRegistration::~ResourceRegistration() = default;

const ResourceType& ModelRegistration::resource_type() const {
    return resource_type_;
};
const API& ModelRegistration::api() const {
    return api_;
};
const Model& ModelRegistration::model() const {
    return model_;
};

void Registry::register_model(std::shared_ptr<ModelRegistration> resource) {
    std::string reg_key = resource->api().to_string() + "/" + resource->model().to_string();
    if (resources_.find(reg_key) != resources_.end()) {
        std::string err = "Cannot add resource with name " + reg_key + "as it already exists";
        throw std::runtime_error(err);
    }

    resources_.emplace(reg_key, resource);
}

void Registry::register_resource(API api,
                                 std::shared_ptr<ResourceRegistration> resource_registration) {
    if (apis_.find(api) != apis_.end()) {
        throw std::runtime_error("Cannot add api " + api.to_string() + " as it already exists");
    }

    apis_.emplace(std::move(api), std::move(resource_registration));
}

std::shared_ptr<ModelRegistration> Registry::lookup_model(std::string name) {
    if (resources_.find(name) == resources_.end()) {
        return nullptr;
    }

    return resources_.at(name);
}

std::shared_ptr<ModelRegistration> Registry::lookup_model(API api, Model model) {
    const std::string name = api.to_string() + "/" + model.to_string();
    return lookup_model(name);
}

std::shared_ptr<ResourceRegistration> Registry::lookup_resource(API api) {
    if (apis_.find(api) == apis_.end()) {
        return nullptr;
    }

    return apis_.at(api);
}

std::unordered_map<std::string, std::shared_ptr<ModelRegistration>> Registry::registered_models() {
    std::unordered_map<std::string, std::shared_ptr<ModelRegistration>> registry;
    for (auto& resource : resources_) {
        registry.emplace(resource.first, resource.second);
    }
    return registry;
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
Status ModelRegistration::create_status(std::shared_ptr<Resource> resource) {
    Status status;
    *status.mutable_name() = resource->get_resource_name(resource->name());
    *status.mutable_status() = google::protobuf::Struct();
    return status;
}

const google::protobuf::ServiceDescriptor* ResourceRegistration::service_descriptor() {
    return service_descriptor_;
}

std::unordered_map<std::string, std::shared_ptr<ModelRegistration>> Registry::resources_;
std::unordered_map<API, std::shared_ptr<ResourceRegistration>> Registry::apis_;

}  // namespace sdk
}  // namespace viam
