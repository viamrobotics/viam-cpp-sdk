#pragma once

#include <module/service.hpp>
#include <unordered_map>

class ModuleManager {
   public:
    ModuleManager();
    std::unordered_map<Name, std::shared_ptr<ModuleService_>> modules;

    std::shared_ptr<ModuleService_> get_module(Component cfg);
    void add_resource(Component cfg, std::vector<std::string> dependencies);
    void reconfigure_resource(Component cfg, std::vector<std::string> dependencies);
    void remove_resource(Name name);
};

