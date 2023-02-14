#pragma once

#include <module/v1/module.grpc.pb.h>

#include <config/resource.hpp>
#include <module/handler_map.hpp>
#include <mutex>
#include <rpc/dial.hpp>

/// NOTE: This is not currently being used, may be useful for testing or down the line but for now
/// clients should not be looking to make use of this.

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
};

