#pragma once

#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

class ResourceServerBase {
   public:
    virtual void register_server(std::shared_ptr<Server> server) = 0;
    const std::shared_ptr<ResourceManager>& resource_manager() const;

   protected:
    ResourceServerBase(std::shared_ptr<ResourceManager> manager) : manager_(manager){};

   private:
    std::shared_ptr<ResourceManager> manager_;
};

}  // namespace sdk
}  // namespace viam
