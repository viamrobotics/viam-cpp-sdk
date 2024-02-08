#pragma once

#include <viam/sdk/resource/resource_manager.hpp>

namespace viam {
namespace sdk {

class ResourceServer {
   public:
    const std::shared_ptr<ResourceManager>& resource_manager() const;

   protected:
    ResourceServer(std::shared_ptr<ResourceManager> manager) : manager_(manager){};

   private:
    std::shared_ptr<ResourceManager> manager_;
};

}  // namespace sdk
}  // namespace viam
