#pragma once

#include <module/service.hpp>
#include <unordered_map>

/// NOTE: This is not currently being used, may be useful for testing or down the line but for now
/// clients should not be looking to make use of this.
class ModuleService_;
class ModuleManager {
   public:
    ModuleManager();
    std::unordered_map<Name, std::shared_ptr<ModuleService_>> modules;

    std::shared_ptr<ModuleService_> get_module(Component cfg);
    void add_resource(Component cfg, std::vector<std::string> dependencies);
    void reconfigure_resource(Component cfg, std::vector<std::string> dependencies);
    void remove_resource(Name name);
};

