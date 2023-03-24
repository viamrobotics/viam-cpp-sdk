#include <components/component_base.hpp>
#include <resource/resource_manager.hpp>
#include <services/service_base.hpp>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

ResourceManager::ResourceManager() {}
std::unordered_map<std::string, std::shared_ptr<ResourceBase>>
    ResourceManager::resources;

// Register a new resource with the registry.
// Resources may not have the same name.
ResourceManager::ResourceManager(
    std::vector<std::shared_ptr<ResourceBase>> resources) {
  for (auto &resource : resources) {
    register_resource(resource);
  }
}

void ResourceManager::register_resource(
    std::shared_ptr<ResourceBase> resource) {
  if (resources.find(resource->name) != resources.end()) {
    std::string err = "Cannot add resource with name " + resource->name +
                      " as it already exists.";
    throw std::runtime_error(err);
  }

  resources[resource->name] = resource;
}

std::shared_ptr<ResourceBase> ResourceManager::get_resource(
    std::string name, ResourceType of_type) {
  if (resources.find(name) == resources.end()) {
    throw "Resource name " + name + " doesn't exist!";
  }

  std::shared_ptr<ResourceBase> resource = resources.at(name);
  if (resource->type == of_type) {
    return resource;
  }

  ResourceType base = ResourceType("ResourceBase");
  if (of_type == base) {
    return resource;
  }
  throw "Resource name " + name +
      " was found, but it has the wrong type! Expected type: " + of_type.type +
      ". Actual type: " + resource->type.type;
}
