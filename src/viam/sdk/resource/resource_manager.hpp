#pragma once

#include <string>
#include <unordered_map>

#include <viam/sdk/resource/resource_base.hpp>
#include <viam/sdk/resource/resource_type.hpp>

namespace viam {
namespace sdk {

class ResourceManager {
   public:
    std::unordered_map<std::string, std::shared_ptr<ResourceBase>> resources;
    void register_resource(std::shared_ptr<ResourceBase> Resource);

    /// returns a resource from the registry.
    /// Args:
    /// 	std::string name: the name of the resource
    ///
    /// Raises:
    /// 	If the name is not within the ResourceManager or the registered
    ///     resource's type is not the expected type, then get_resource
    ///     will throw an error.
    std::shared_ptr<ResourceBase> get_resource(std::string name, ResourceType of_type);
    ResourceManager();

   private:
    ResourceManager(std::vector<std::shared_ptr<ResourceBase>> resources);
};

}  // namespace sdk
}  // namespace viam
