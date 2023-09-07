#pragma once

#include <signal.h>

#include <viam/api/component/generic/v1/generic.grpc.pb.h>
#include <viam/api/module/v1/module.grpc.pb.h>

#include <viam/sdk/module/module.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

class ModuleService_ : public viam::module::v1::ModuleService::Service {
   public:
    void start(std::shared_ptr<Server> server);
    void close();
    std::shared_ptr<Resource> get_parent_resource(Name name);

    void add_api_from_registry(std::shared_ptr<Server> server, API api);
    void add_model_from_registry(std::shared_ptr<Server> server, API api, Model model);
    ::grpc::Status AddResource(::grpc::ServerContext* context,
                               const ::viam::module::v1::AddResourceRequest* request,
                               ::viam::module::v1::AddResourceResponse* response) override;

    ::grpc::Status ReconfigureResource(
        ::grpc::ServerContext* context,
        const ::viam::module::v1::ReconfigureResourceRequest* request,
        ::viam::module::v1::ReconfigureResourceResponse* response) override;

    ::grpc::Status RemoveResource(::grpc::ServerContext* context,
                                  const ::viam::module::v1::RemoveResourceRequest* request,
                                  ::viam::module::v1::RemoveResourceResponse* response) override;

    ::grpc::Status Ready(::grpc::ServerContext* context,
                         const ::viam::module::v1::ReadyRequest* request,
                         ::viam::module::v1::ReadyResponse* response) override;

    ::grpc::Status ValidateConfig(::grpc::ServerContext* context,
                                  const ::viam::module::v1::ValidateConfigRequest* request,
                                  ::viam::module::v1::ValidateConfigResponse* response) override;

    ModuleService_(std::string addr);
    ~ModuleService_();

   private:
    void add_model_from_registry_inlock_(std::shared_ptr<Server> server,
                                         API api,
                                         Model model,
                                         const std::lock_guard<std::mutex>&);
    void add_api_from_registry_inlock_(std::shared_ptr<Server> server,
                                       API api,
                                       const std::lock_guard<std::mutex>& lock);
    std::mutex lock_;
    std::shared_ptr<Module> module_;
    std::shared_ptr<RobotClient> parent_;
    std::string parent_addr_;
};

// C++ modules must handle SIGINT and SIGTERM. Make sure to create a sigset
// for SIGINT and SIGTERM that can be later awaited in a thread that cleanly
// shuts down your module. pthread_sigmask should be called near the start of
// main so that later threads inherit the mask. SignalManager can handle the
// boilerplate of this functionality.
class SignalManager {
   public:
    SignalManager();
    // wait on sigset
    int wait(int* sig);

   private:
    sigset_t sigset;
};

}  // namespace sdk
}  // namespace viam
