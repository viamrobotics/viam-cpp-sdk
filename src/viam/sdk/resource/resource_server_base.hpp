#pragma once

#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/subtype/subtype.hpp>

namespace viam {
namespace sdk {

class ResourceServerBase {
   public:
    virtual void register_server(std::shared_ptr<Server> server) = 0;
    const std::shared_ptr<SubtypeService> get_sub_svc() const;

   protected:
    ResourceServerBase(std::shared_ptr<SubtypeService> sub_svc) : sub_svc_(sub_svc){};

   private:
    std::shared_ptr<SubtypeService> sub_svc_;
};

}  // namespace sdk
}  // namespace viam
