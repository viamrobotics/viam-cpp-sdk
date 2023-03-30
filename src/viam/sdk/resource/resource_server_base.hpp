#pragma once

#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/subtype/subtype.hpp>

namespace viam {
namespace sdk {

class ResourceServerBase {
   public:
    virtual void register_server(std::shared_ptr<Server> server) = 0;

   private:
    std::shared_ptr<SubtypeService> sub_svc;
};

}  // namespace sdk
}  // namespace viam
