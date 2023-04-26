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
    void set_ready();
    Module(std::string addr);
    std::lock_guard<std::mutex> lock();
    const std::string& name() const;
    const std::string& addr() const;
    const bool ready() const;
    HandlerMap_& handles();
    std::shared_ptr<grpc::Channel> channel();
    std::unordered_map<Subtype, std::shared_ptr<SubtypeService>> services();
    std::vector<std::shared_ptr<ResourceServerBase>> servers();

   private:
    std::mutex lock_;
    std::string name_;
    std::string addr_;
    bool ready_;
    HandlerMap_ handles_;
    std::shared_ptr<grpc::Channel> channel_;
    std::unordered_map<Subtype, std::shared_ptr<SubtypeService>> services_;
    std::vector<std::shared_ptr<ResourceServerBase>> servers_;
};

}  // namespace sdk
}  // namespace viam
