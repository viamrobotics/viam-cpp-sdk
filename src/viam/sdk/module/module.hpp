#pragma once

#include <viam/sdk/module/handler_map.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/subtype/subtype.hpp>

namespace viam {
namespace sdk {

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

}  // namespace sdk
}  // namespace viam
