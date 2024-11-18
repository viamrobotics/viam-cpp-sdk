#pragma once

#include <viam/api/component/generic/v1/generic.grpc.pb.h>
#include <viam/api/module/v1/module.grpc.pb.h>

#include <viam/sdk/module/module.hpp>
#include <viam/sdk/module/signal_manager.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

/// @defgroup Module Classes related to C++ module development.

/// @class ModuleService
/// @brief Defines the gRPC receiving logic for a module. C++ module authors
/// can construct a ModuleService and use its associated methods to write
/// a working C++ module. See examples under `src/viam/examples/modules`.
/// @ingroup Module
class ModuleService : viam::module::v1::ModuleService::Service {
   public:
    /// @brief Creates a new ModuleService that can serve on the provided socket.
    /// @param addr Address of socket to serve on.
    explicit ModuleService(std::string addr);

    /// @brief Creates a new ModuleService. Socket path and log level will be
    /// inferred from passed in command line arguments, and passed in model
    /// registrations will be registered and added to module.
    /// @param argc Number of arguments from command line.
    /// @param argv Arguments from command line.
    /// @param registrations Models to register and add to the module.
    explicit ModuleService(int argc,
                           char** argv,
                           const std::vector<std::shared_ptr<ModelRegistration>>& registrations);
    ~ModuleService();

    /// @brief Starts module. serve will return when SIGINT or SIGTERM is received
    /// (this happens when the RDK shuts down).
    void serve();

    /// @brief Adds an API/model pair to the module; both the API and model should have
    /// already been registered. If the ModuleService was constructed with a vector
    /// of ModelRegistration, the passed in models will already be registered and added.
    /// @param api The API to add.
    /// @param model The model to add.
    void add_model_from_registry(API api, Model model);

   private:
    void init_logging_();
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
    Dependencies get_dependencies_(google::protobuf::RepeatedPtrField<std::string> const& proto,
                                   std::string const& resource_name);
    std::shared_ptr<Resource> get_parent_resource_(const Name& name);

    std::unique_ptr<Logger> logger_;
    std::mutex lock_;
    struct impl;
    std::shared_ptr<RobotClient> parent_;
    std::unique_ptr<impl> impl_;
    std::unique_ptr<Module> module_;
    std::string parent_addr_;
    std::unique_ptr<Server> server_;
    SignalManager signal_manager_;
};

}  // namespace sdk
}  // namespace viam
