#pragma once

#include <module/handler_map.hpp>
#include <resource/resource.hpp>
#include <resource/resource_server_base.hpp>
#include <robot/client.hpp>
#include <subtype/subtype.hpp>

class Module {
   public:
    std::mutex lock;
    std::string name;
    std::string addr;
    bool ready;
    HandlerMap_ handles;
    std::shared_ptr<grpc::Channel> channel;
    std::unordered_map<Subtype, std::shared_ptr<SubtypeService>> services;
    std::vector<std::shared_ptr<ResourceServerBase>> servers;
    void set_ready();
    Module(std::string addr);
};

