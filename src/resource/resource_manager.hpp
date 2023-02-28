#pragma once

#include <resource/resource_base.hpp>
#include <resource/resource_type.hpp>
#include <string>
#include <unordered_map>

class ResourceManager {
   public:
    void register_resource(std::shared_ptr<ResourceBase> Resource);

    /// returns a resource from the registry.
    /// Args:
    /// 	std::string name: the name of the resource
    ///
    /// Raises:
    /// 	If the name is not within the ResourceManager or the registered
    /// resource's type is not the expected type, then get_resource
    /// will throw an error.
    std::shared_ptr<ResourceBase> get_resource(std::string name, ResourceType of_type);
    ResourceManager();

   private:
    static std::unordered_map<std::string, std::shared_ptr<ResourceBase>> resources;
    ResourceManager(std::vector<std::shared_ptr<ResourceBase>> resources);
};

