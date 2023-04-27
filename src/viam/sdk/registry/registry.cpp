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

#include <viam/sdk/components/component_base.hpp>
#include <viam/sdk/components/generic/generic.hpp>
#include <viam/sdk/components/service_base.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_base.hpp>
#include <viam/sdk/resource/resource_type.hpp>
#include <viam/sdk/services/service_base.hpp>

namespace viam {
namespace sdk {

using viam::robot::v1::Status;

void Registry::register_resource(std::shared_ptr<ModelRegistration> resource) {
    std::string reg_key = resource->subtype.to_string() + "/" + resource->model.to_string();
    if (resources.find(reg_key) != resources.end()) {
        std::string err = "Cannot add resource with name " + reg_key + "as it already exists";
        throw std::runtime_error(err);
    }

    resources.emplace(reg_key, resource);
}

void Registry::register_subtype(Subtype subtype,
                                std::shared_ptr<ResourceSubtype> resource_subtype) {
    if (subtypes.find(subtype) != subtypes.end()) {
        throw std::runtime_error("Cannot add subtype " + subtype.to_string() +
                                 " as it already exists");
    }

    subtypes.emplace(std::move(subtype), std::move(resource_subtype));
}

std::shared_ptr<ModelRegistration> Registry::lookup_resource(std::string name) {
    if (resources.find(name) == resources.end()) {
        return nullptr;
    }

    return resources.at(name);
}

std::shared_ptr<ModelRegistration> Registry::lookup_resource(Subtype subtype, Model model) {
    const std::string name = subtype.to_string() + "/" + model.to_string();
    return lookup_resource(name);
}

std::shared_ptr<ResourceSubtype> Registry::lookup_subtype(Subtype subtype) {
    if (subtypes.find(subtype) == subtypes.end()) {
        return nullptr;
    }

    return subtypes.at(subtype);
}

std::unordered_map<std::string, std::shared_ptr<ModelRegistration>>
Registry::registered_resources() {
    std::unordered_map<std::string, std::shared_ptr<ModelRegistration>> registry;
    for (auto& resource : resources) {
        registry.emplace(resource.first, resource.second);
    }
    return registry;
}

Status ModelRegistration::create_status(std::shared_ptr<ResourceBase> resource) {
    Status status;
    *status.mutable_name() = resource->get_resource_name(resource->name());
    *status.mutable_status() = google::protobuf::Struct();
    return status;
}

std::unordered_map<std::string, std::shared_ptr<ModelRegistration>> Registry::resources;
std::unordered_map<Subtype, std::shared_ptr<ResourceSubtype>> Registry::subtypes;

}  // namespace sdk
}  // namespace viam
