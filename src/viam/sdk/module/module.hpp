#pragma once

#include <viam/sdk/module/handler_map.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

class Module {
   public:
    void set_ready();
    Module(std::string addr);
    const std::string& name() const;
    const std::string& addr() const;
    bool ready() const;
    const HandlerMap_& handles() const;
    HandlerMap_& mutable_handles();
    const std::shared_ptr<grpc::Channel>& channel() const;

   private:
    std::string name_;
    std::string addr_;
    bool ready_;
    HandlerMap_ handles_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam
