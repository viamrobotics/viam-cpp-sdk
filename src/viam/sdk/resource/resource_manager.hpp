/// @file resource/resource_manager.hpp
///
/// @brief Defines a general-purpose resource manager.
#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

#include <boost/optional/optional.hpp>
#include <grpcpp/impl/service_type.h>

#include <viam/api/component/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdk {

/// @class ResourceManager
/// @brief Defines a resource manager for use by anything that tracks resources.
class ResourceManager {
   public:
    /// @brief Returns a resource.
    /// @param name the name of the desired resource.
    /// @throws `std::runtime_error` if the desired resource does not exist.
    std::shared_ptr<Resource> resource(const std::string& name);

    /// @brief Returns a resource after dynamically downcasting to `T`.
    /// @param name of the desired resource.
    /// @throws `std::runtime_error` if the desired resource does not exist.
    template <typename T>
    std::shared_ptr<T> resource(const std::string& name) {
        static_assert(std::is_base_of<Resource, T>::value, "T is not derived from Resource");
        return std::dynamic_pointer_cast<T>(resource(name));
    }

    /// @brief Replaces all resources in the manager.
    /// @param resources The resources to replace with.
    void replace_all(const std::unordered_map<Name, std::shared_ptr<Resource>>& resources);

    /// @brief Adds a single resource to the manager.
    /// @param name The name of the resource.
    /// @param resource The resource being added.
    void add(const Name& name, std::shared_ptr<Resource> resource);

    /// @brief Adds a single resource to the manager.
    /// @param name The name of the resource.
    /// @param resource The resource being added.
    void add(std::string name, std::shared_ptr<Resource> resource);

    /// @brief Remodes a single resource from the manager.
    /// @param name The name of the resource to remove.
    void remove(const Name& name);

    /// @brief Replaces an existing resource. No-op if the named resource does not exist.
    /// @param name The name of the resource to replace.
    /// @param resource The new resource that is replacing the existing one.
    void replace_one(const Name& name, std::shared_ptr<Resource> resource);

    /// @brief Returns a reference to the existing resources within the manager.
    const std::unordered_map<std::string, std::shared_ptr<Resource>>& resources() const;

    ResourceManager(){};

   private:
    std::mutex lock_;
    std::unordered_map<std::string, std::shared_ptr<Resource>> resources_;
    /// @brief `short_names_` is a shortened version of `Name` N of form <remote>:<name>.
    std::unordered_map<std::string, std::string> short_names_;
    void do_add(const Name& name, std::shared_ptr<Resource> resource);
    void do_add(std::string name, std::shared_ptr<Resource> resource);
    void do_remove(const Name& name);
};

}  // namespace sdk
}  // namespace viam
