#pragma once

#include <config/resource.hpp>
#include <registry/registry.hpp>
#include <resource/reconfigurable_resource.hpp>

// CR erodkin: instead of using the actual type name, we should have `Dependencies` defined in a new
// location so we don't get a dependency loop on defining it between this file and resource.hpp
class ReconfigurableService : public ServiceBase, public ReconfigurableResource {
   public:
    std::function<void(Service, std::unordered_map<Name, ResourceBase>)> reconfigure;
};

