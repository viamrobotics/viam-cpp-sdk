#pragma once

#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

class ResourceServer {
   public:
    const std::shared_ptr<ResourceManager>& resource_manager() const;

   protected:
    ResourceServer(std::shared_ptr<ResourceManager> manager,
                   std::shared_ptr<Server> server,
                   grpc::Service* service)
        : manager_(manager), server_(server) {
        server->register_service(service);
    };

   private:
    std::shared_ptr<ResourceManager> manager_;
    std::shared_ptr<Server> server_;
};

}  // namespace sdk
}  // namespace viam
