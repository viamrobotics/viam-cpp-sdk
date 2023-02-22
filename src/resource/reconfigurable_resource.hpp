#pragma once

#include <resource/resource.hpp>
#include <resource/resource_base.hpp>

class ReconfigurableResource : public ResourceBase {
   public:
    std::function<Name()> name;
    std::function<void(ReconfigurableResource)> reconfigure;
};
