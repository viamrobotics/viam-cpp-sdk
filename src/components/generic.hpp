#pragma once

#include <common/utils.hpp>
#include <resource/resource.hpp>

#include "registry/registry.hpp"

class Generic {
   public:
    static std::shared_ptr<ResourceSubtype> resource_subtype();
    static Subtype subtype();
};

