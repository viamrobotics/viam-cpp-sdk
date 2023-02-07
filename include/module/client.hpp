#pragma once

#include <module/v1/module.grpc.pb.h>

#include <config/resource.hpp>
#include <module/handler_map.hpp>
#include <mutex>
#include <rpc/dial.hpp>

class ModuleClient {
   public:
    std::mutex lock;
    std::unique_ptr<viam::module::v1::ModuleService::Stub> stub_;
    HandlerMap_ handles;

    ModuleClient(Viam::SDK::ViamChannel channel);
    void add_resource(Component cfg, std::vector<std::string> dependencies);

    void reconfigure_resource(Component cfg, std::vector<std::string> dependencies);
    void remove_resource(Name name);

    void ready(std::string address);
    // CR erodkin: api for module is defined in proto?? so says cheuk but that seems weird
};

