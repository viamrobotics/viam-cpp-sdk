#pragma once

#include <subtype/subtype.hpp>
#include <rpc/server.hpp>

class ResourceServerBase {
   public:
    virtual void register_server(Server* server) = 0;

   private:
    std::shared_ptr<SubtypeService> sub_svc;
};
