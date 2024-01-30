#pragma once

#include <viam/sdk/resource/resource_manager.hpp>

namespace viam {
namespace sdk {

class ResourceServer {
   public:
    virtual API api() const = 0;
    const std::shared_ptr<ResourceManager>& resource_manager() const;
    std::shared_ptr<ResourceManager>& resource_manager();

   protected:
    ResourceServer(std::shared_ptr<ResourceManager> manager) : manager_(manager){};

   private:
    std::shared_ptr<ResourceManager> manager_;
};

}  // namespace sdk
}  // namespace viam
