#pragma once

#include "viam/sdk/registry/registry.hpp"
#include <signal.h>

#include <viam/api/component/generic/v1/generic.grpc.pb.h>
#include <viam/api/module/v1/module.grpc.pb.h>

#include <viam/sdk/module/module.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

/// @defgroup Module Classes related to C++ module development.

/// @class SignalManager
/// @brief Defines handling logic for SIGINT and SIGTERM required by all C++
/// modules.
/// @ingroup Module
class SignalManager {
   public:
    explicit SignalManager();

    /// @brief Wait for SignalManager to receive SIGINT or SIGTERM.
    /// @param sig Location reference where the signal number is stored.
    /// @return The signal number if successful, -1 if not.
    int wait(int* sig);

   private:
    sigset_t sigset_;
};

/// @class ModuleService
/// @brief Defines the gRPC receiving logic for a module. C++ module authors
/// can construct a ModuleService and use its associated methods to write
/// a working C++ module. See examples under `src/viam/examples/modules`.
/// @ingroup Module
class ModuleService : public viam::module::v1::ModuleService::Service {
   public:
    explicit ModuleService(std::string addr);
    explicit ModuleService(int argc,
                           char** argv,
                           std::vector<std::shared_ptr<ModelRegistration>> registrations);
    ~ModuleService();

    /// @brief Starts module. If module was constructed with a vector of ModelRegistration,
    /// serve will return when SIGINT or SIGTERM is received (this happens when the RDK
    /// shuts down).
    void serve();
    /// @brief Adds an API to the module that has already been registered.
    /// @param api The API to add.
    void add_api_from_registry(API api);
    /// @brief Adds an API/model pair to the module; both the API and model should have
    /// already been registered.
    /// @param api The API to add.
    /// @param model The model to add.
    void add_model_from_registry(API api, Model model);

   private:
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

    void add_model_from_registry_inlock_(API api, Model model, const std::lock_guard<std::mutex>&);
    void add_api_from_registry_inlock_(API api, const std::lock_guard<std::mutex>& lock);
    Dependencies get_dependencies(google::protobuf::RepeatedPtrField<std::string> const& proto,
                                  std::string const& resource_name);
    std::shared_ptr<Resource> get_parent_resource(Name name);

    std::mutex lock_;
    std::shared_ptr<Module> module_;
    std::shared_ptr<RobotClient> parent_;
    std::string parent_addr_;
    std::shared_ptr<Server> server_;
    std::shared_ptr<SignalManager> signal_manager_;
};

}  // namespace sdk
}  // namespace viam
