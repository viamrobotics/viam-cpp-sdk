#pragma once

#include <subtype/subtype.hpp>

class ResourceServerBase {
   public:
    virtual void register_server() = 0;

   private:
    std::shared_ptr<SubtypeService> sub_svc;
};
