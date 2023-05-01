/// @file resource/resource_manager.hpp
///
/// @brief Defines a general-purpose resource manager.
#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <boost/optional/optional.hpp>
#include <grpcpp/impl/service_type.h>

#include <viam/api/component/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_base.hpp>

namespace viam {
namespace sdk {

/// @class ResourceManager
/// @brief Defines a resource manager for use by anything that tracks resources.
class ResourceManager {
   public:
    /// @brief Returns a resource.
    /// @param name the name of the desired resource.
    /// @throws `std::runtime_error` if the desired resource does not exist.
    std::shared_ptr<ResourceBase> resource(std::string name);

    /// @brief Replaces all resources in the manager.
    /// @param resources The resources to replace with.
    void replace_all(std::unordered_map<Name, std::shared_ptr<ResourceBase>> resources);

    /// @brief Adds a single resource to the manager.
    /// @param name The name of the resource.
    /// @param resource The resource being added.
    void add(Name name, std::shared_ptr<ResourceBase> resource);

    /// @brief Adds a single resource to the manager.
    /// @param name The name of the resource.
    /// @param resource The resource being added.
    void add(std::string name, std::shared_ptr<ResourceBase> resource);

    /// @brief Remodes a single resource from the manager.
    /// @param name The name of the resource to remove.
    void remove(Name name);

    /// @brief Replaces an existing resource. No-op if the named resource does not exist.
    /// @param name The name of the resource to replace.
    /// @param resource The new resource that is replacing the existing one.
    void replace_one(Name name, std::shared_ptr<ResourceBase> resource);

    /// @brief Returns a reference to the existing resources within the manager.
    const std::unordered_map<std::string, std::shared_ptr<ResourceBase>>& resources() const;

    ResourceManager(){};

   private:
    std::mutex lock_;
    std::unordered_map<std::string, std::shared_ptr<ResourceBase>> resources_;
    /// @brief `short_names_` is a shortened version of `Name` N of form <remote>:<name>.
    std::unordered_map<std::string, std::string> short_names_;
    void do_add(Name name, std::shared_ptr<ResourceBase> resource);
    void do_add(std::string name, std::shared_ptr<ResourceBase> resource);
    void do_remove(Name name);
};

}  // namespace sdk
}  // namespace viam
