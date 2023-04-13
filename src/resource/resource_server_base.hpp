#pragma once

#include <rpc/server.hpp>
#include <subtype/subtype.hpp>

namespace viam {
namespace cppsdk {

class ResourceServerBase {
   public:
    virtual void register_server(std::shared_ptr<Server> server) = 0;

   private:
    std::shared_ptr<SubtypeService> sub_svc;
};

}  // namespace cppsdk
}  // namespace viam
