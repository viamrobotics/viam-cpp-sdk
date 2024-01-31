/// @file registry/registry.hpp
///
/// @brief Defines the resource registry and associated types.
#pragma once

#include <string>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <grpcpp/channel.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/server.h>

#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

// TODO(RSDK-3030): instead of std::functions, consider making these functions
// virtual
// TODO(RSDK-3030): one class per header
/// @class ResourceRegistration registry.hpp "registry/registry.hpp"
/// @brief Defines registered `Resource` creation functionality.
class ResourceRegistration {
   public:
    virtual ~ResourceRegistration();

    // TODO(RSDK-6484): the semantics of `create_reconfigurable` are opaque (and to-date
    // are in fact non-existent). We should get rid of this and just add a `Reconfigurable`
    // base class (similar to `Stoppable`), and things that want to reconfigure can inherit
    // from it.
    /// @brief Add `Reconfigure` functionality to a resource.
    std::function<std::shared_ptr<Resource>(std::shared_ptr<Resource>, Name)> create_reconfigurable;

    /// @brief Create a resource's gRPC server.
    /// @param manager The server's `ResourceManager`.
    /// @param server The Server with which to register the relevant gRPC service.
    /// @return a `shared_ptr` to the gRPC server.
    virtual std::shared_ptr<ResourceServer> create_resource_server(
        std::shared_ptr<ResourceManager> manager, Server& server) = 0;

    /// @brief Returns a reference to the `ResourceRegistration`'s service descriptor.
    const google::protobuf::ServiceDescriptor* service_descriptor();

    /// @brief Create gRPC client to a resource.
    /// @param name The name of the resource.
    /// @param channel A channel connected to the client.
    /// @return A `shared_ptr` to the resource client.
    virtual std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                        std::shared_ptr<grpc::Channel> channel) = 0;

    ResourceRegistration(const google::protobuf::ServiceDescriptor* service_descriptor)
        : service_descriptor_(service_descriptor){};

   private:
    const google::protobuf::ServiceDescriptor* service_descriptor_;
};

/// @class ModelRegistration
/// @brief Information about a registered model, including a constructor and config validator.
class ModelRegistration {
   public:
    ModelRegistration(
        API api,
        Model model,
        std::function<std::shared_ptr<Resource>(Dependencies, ResourceConfig)> constructor)
        : construct_resource(std::move(constructor)),
          validate(default_validator),
          model_(std::move(model)),
          api_(std::move(api)){};

    ModelRegistration(
        API api,
        Model model,
        std::function<std::shared_ptr<Resource>(Dependencies, ResourceConfig)> constructor,
        std::function<std::vector<std::string>(ResourceConfig)> validator)
        : construct_resource(std::move(constructor)),
          validate(std::move(validator)),
          model_(std::move(model)),
          api_(std::move(api)){};

    const API& api() const;
    const Model& model() const;

    /// @brief Constructs a resource from a map of dependencies and a resource config.
    std::function<std::shared_ptr<Resource>(Dependencies, ResourceConfig)> construct_resource;

    /// @brief Validates a resource config.
    /// @return a list of the resource's implicit dependencies.
    /// @throws Can throw exceptions, which will be returned to the parent via gRPC.
    std::function<std::vector<std::string>(ResourceConfig)> validate;

    /// @brief Creates a `Status` object for a given resource.
    viam::robot::v1::Status create_status(std::shared_ptr<Resource> resource);

   private:
    // default_validator is the default validator for all models if no validator
    // is provided in construction. No dependencies are returned.
    Model model_;
    API api_;
    static std::vector<std::string> default_validator(ResourceConfig cfg) {
        return {};
    };
};

/// @class Registry
/// @brief A registry of known resources.
class Registry {
   public:
    /// @brief Registers a resource with the Registry.
    /// @param resource An object containing resource registration information.
    /// @throws `std::runtime_error` if the resource has already been registered.
    /// @throws `std::runtime_error` if `initialize` has not been called.
    static void register_model(std::shared_ptr<ModelRegistration> resource);

    /// @brief Lookup a given registered resource.
    /// @param name The name of the resource to lookup.
    /// @return a `shared_ptr` to the resource's registration data.
    /// @throws `std::runtime_error` if `initialize` has not been called.
    static std::shared_ptr<ModelRegistration> lookup_model(std::string name);

    /// @brief Lookup a given registered resource.
    /// @param api The api of the resource to lookup.
    /// @param model The model of the resource to lookup.
    /// @return a `shared_ptr` to the resource's registration data.
    /// @throws `std::runtime_error` if `initialize` has not been called.
    static std::shared_ptr<ModelRegistration> lookup_model(API api, Model model);

    /// @brief Register an api.
    /// @param api The api to be registered.
    /// @throws `std::runtime_error` if `initialize` has not been called.
    template <typename ResourceClientT, typename ResourceServerT, typename ProtoServiceT>
    static void register_resource(API api) {
        class ResourceRegistration2 final : public ResourceRegistration {
           public:
            using ResourceRegistration::ResourceRegistration;
            std::shared_ptr<ResourceServer> create_resource_server(
                std::shared_ptr<ResourceManager> manager, Server& server) override {
                auto rs = std::make_shared<ResourceServerT>(manager);
                server.register_service(rs.get());
                return rs;
            }

            std::shared_ptr<Resource> create_rpc_client(
                std::string name, std::shared_ptr<grpc::Channel> chan) override {
                return std::make_shared<ResourceClientT>(std::move(name), std::move(chan));
            }
        };

        const google::protobuf::ServiceDescriptor* sd =
            get_service_descriptor_(ProtoServiceT::service_full_name());
        Registry::register_resource(api, std::make_shared<ResourceRegistration2>(sd));
    }

    /// @brief Register an api.
    /// @param api The api to be registered.
    /// @param resource_registration `ResourceRegistration` with resource functionality.
    /// @throws `std::runtime_error` if `initialize` has not been called.
    static void register_resource(API api,
                                  std::shared_ptr<ResourceRegistration> resource_registration);

    /// @brief Lookup a registered api.
    /// @param api The api to lookup.
    /// @return A `shared_ptr` to the registered api's `ResourceRegistration`.
    /// @throws `std::runtime_error` if `initialize` has not been called.
    static std::shared_ptr<ResourceRegistration> lookup_resource(API api);

    /// @brief Provide information on registered resource models.
    /// @return A map from name to `ModelRegistration` of all registered resource models.
    /// @throws `std::runtime_error` if `initialize` has not been called.
    static const std::unordered_map<std::string, std::shared_ptr<ModelRegistration>>&
    registered_models();

    /// @brief Provide access to registered resources.
    /// @return A map from `API` to `ResourceRegistration` of all registered resources.
    /// @throws `std::runtime_error` if `initialize` has not been called.
    static const std::unordered_map<API, std::shared_ptr<ResourceRegistration>>&
    registered_resources();

    /// @brief Initialized the Viam registry. No-op if it has already been called.
    static void initialize();

   private:
    static bool initialized_;

    static const google::protobuf::ServiceDescriptor* get_service_descriptor_(
        const char* service_full_name);
    static std::unordered_map<std::string, std::shared_ptr<ModelRegistration>> resources_;
    static std::unordered_map<API, std::shared_ptr<ResourceRegistration>> apis_;
};

}  // namespace sdk
}  // namespace viam
