#pragma once

#include <config/resource.hpp>
#include <registry/registry.hpp>
#include <resource/reconfigurable_resource.hpp>

// TODO(RSDK-1742): instead of using the actual type name, we should have `Dependencies` defined in
// a new location so we don't get a dependency loop on defining it between this file and
// registry.hpp
class ReconfigurableService : public ServiceBase, public ReconfigurableResource {
   public:
    std::function<void(::Service, std::unordered_map<Name, std::shared_ptr<ResourceBase>>)>
        reconfigure;
};

