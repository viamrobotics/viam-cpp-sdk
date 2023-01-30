#include <module/manager.hpp>
#include <module/service.hpp>
#include <resource/resource.hpp>

void ModuleManager::reconfigure_resource(Component cfg, std::vector<std::string> dependencies) {
    viam::module::v1::ReconfigureResourceRequest req;
    viam::module::v1::ReconfigureResourceResponse resp;
    grpc::ServerContext ctx;

    for (auto& dep : dependencies) {
        *req.mutable_dependencies()->Add() = dep;
    }
    *req.mutable_config() = cfg.to_proto();
    std::shared_ptr<ModuleService_> ms = this->get_module(cfg);
    ms->ReconfigureResource(&ctx, &req, &resp);
}

void ModuleManager::remove_resource(Name name) {
    viam::module::v1::RemoveResourceRequest req;
    viam::module::v1::RemoveResourceResponse resp;
    grpc::ServerContext ctx;

    *req.mutable_name() = name.to_string();
    std::shared_ptr<ModuleService_> ms = this->modules.at(name);
    ms->RemoveResource(&ctx, &req, &resp);
}

std::shared_ptr<ModuleService_> ModuleManager::get_module(Component cfg) {
    for (auto& mod_server : modules) {
        std::shared_ptr<ModuleService_> ms = mod_server.second;
        if (ms == nullptr) {
            throw "module not found: " + cfg.name;
        }
        std::shared_ptr<Module> module = ms->module;
        for (auto& api : module->handles.handles) {
            const Subtype cfg_subtype = cfg.resource_name();

            if (!(api.first.subtype == cfg_subtype)) {
                continue;
            }
            for (Model& model : module->handles.handles.at(api.first)) {
                if (cfg.model == model) {
                    return ms;
                }
            }
        }
    }
    return nullptr;
};

ModuleManager::ModuleManager(){};

void ModuleManager::add_resource(Component cfg, std::vector<std::string> dependencies) {
    viam::module::v1::AddResourceRequest req;
    viam::module::v1::AddResourceResponse resp;
    grpc::ServerContext ctx;

    viam::app::v1::ComponentConfig proto_cfg = cfg.to_proto();

    for (auto& dep : dependencies) {
        *req.mutable_dependencies()->Add() = dep;
    }
    *req.mutable_config() = proto_cfg;

    std::shared_ptr<ModuleService_> ms = this->get_module(cfg);
    ms->AddResource(&ctx, &req, &resp);
};

