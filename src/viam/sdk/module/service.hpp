#pragma once

#include <signal.h>

#include <viam/sdk/module/module.hpp>
#include <viam/sdk/module/signal_manager.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace google {
namespace protobuf {

template <typename>
class RepeatedPtrField;

}  // namespace protobuf
}  // namespace google

namespace viam {
namespace sdk {

/// @defgroup Module Classes related to C++ module development.

/// @class ModuleService
/// @brief Defines the gRPC receiving logic for a module. C++ module authors
/// can construct a ModuleService and use its associated methods to write
/// a working C++ module. See examples under `src/viam/examples/modules`.
/// @ingroup Module
class ModuleService {
   public:
    /// @brief Creates a new ModuleService that can serve on the provided socket.
    /// @param addr Address of socket to serve on.
    explicit ModuleService(std::string addr, Registry* registry);

    /// @brief Creates a new ModuleService. Socket path and log level will be
    /// inferred from passed in command line arguments, and passed in model
    /// registrations will be registered and added to module.
    /// @param argc Number of arguments from command line.
    /// @param argv Arguments from command line.
    /// @param registrations Models to register and add to the module.
    explicit ModuleService(int argc,
                           char** argv,
                           const std::vector<std::shared_ptr<ModelRegistration>>& registrations,
                           Registry* registry);
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
    struct ServiceImpl;
    friend ModuleService::ServiceImpl;

    void add_model_from_registry_inlock_(API api, Model model, const std::lock_guard<std::mutex>&);
    Dependencies get_dependencies_(google::protobuf::RepeatedPtrField<std::string> const* proto,
                                   std::string const& resource_name);
    std::shared_ptr<Resource> get_parent_resource_(const Name& name);

    Registry* registry_;

    std::mutex lock_;
    std::unique_ptr<Module> module_;
    std::shared_ptr<RobotClient> parent_;
    std::string parent_addr_;
    std::unique_ptr<Server> server_;
    SignalManager signal_manager_;

    std::unique_ptr<ServiceImpl> impl_;
};

}  // namespace sdk
}  // namespace viam
