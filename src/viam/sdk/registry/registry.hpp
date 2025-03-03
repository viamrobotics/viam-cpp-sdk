/// @file registry/registry.hpp
///
/// @brief Defines the resource registry and associated types.
#pragma once

#include <string>

#include <viam/sdk/common/grpc_fwd.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace google {
namespace protobuf {

class ServiceDescriptor;

}
}  // namespace google

namespace viam {
namespace sdk {

// TODO(RSDK-6617): one class per header
class ResourceServerRegistration {
   public:
    ResourceServerRegistration(const ::google::protobuf::ServiceDescriptor* service_descriptor);

    virtual ~ResourceServerRegistration();

    /// @brief Create a resource's gRPC server.
    /// @param manager The server's `ResourceManager`.
    /// @param server The Server with which to register the relevant gRPC service.
    /// @return a `shared_ptr` to the gRPC server.
    virtual std::shared_ptr<ResourceServer> create_resource_server(
        std::shared_ptr<ResourceManager> manager, Server& server) const = 0;

    /// @brief Returns a reference to the `ResourceServerRegistration`'s service descriptor.
    const ::google::protobuf::ServiceDescriptor* service_descriptor() const;

   private:
    const ::google::protobuf::ServiceDescriptor* service_descriptor_;
};

/// @class ResourceClientRegistration registry.hpp "registry/registry.hpp"
/// @brief Defines registered `Resource` client creation functionality.
class ResourceClientRegistration {
   public:
    ResourceClientRegistration() = default;

    virtual ~ResourceClientRegistration();

    /// @brief Create gRPC client to a resource.
    /// @param name The name of the resource.
    /// @param channel A channel connected to the client.
    /// @return A `shared_ptr` to the resource client.
    virtual std::shared_ptr<Resource> create_rpc_client(
        std::string name, std::shared_ptr<GrpcChannel> channel) const = 0;
};

// TODO(RSDK-6616): instead of std::functions, consider making these functions
// virtual
/// @class ModelRegistration
/// @brief Information about a registered model, including a constructor and config validator.
class ModelRegistration {
   public:
    ModelRegistration(
        API api,
        Model model,
        std::function<std::shared_ptr<Resource>(Dependencies, ResourceConfig)> constructor);

    ModelRegistration(
        API api,
        Model model,
        std::function<std::shared_ptr<Resource>(Dependencies, ResourceConfig)> constructor,
        std::function<std::vector<std::string>(ResourceConfig)> validator);

    const API& api() const;
    const Model& model() const;

    /// @brief Constructs a resource from a map of dependencies and a resource config.
    std::function<std::shared_ptr<Resource>(Dependencies, ResourceConfig)> construct_resource;

    /// @brief Validates a resource config.
    /// @return a list of the resource's implicit dependencies.
    /// @throws Can throw exceptions, which will be returned to the parent via gRPC.
    std::function<std::vector<std::string>(ResourceConfig)> validate;

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
    /// @brief Get the application-wide instance of Registry.
    static Registry& get();

    /// @brief Registers a resource with the Registry.
    /// @param resource An object containing resource registration information.
    /// @throws `Exception` if the resource has already been registered.
    void register_model(std::shared_ptr<const ModelRegistration> resource);

    /// @brief Lookup a given registered resource.
    /// @param name The name of the resource to lookup.
    /// @return a `shared_ptr` to the resource's registration data.
    std::shared_ptr<const ModelRegistration> lookup_model(const std::string& name) const;

    /// @brief Lookup a given registered resource.
    /// @param api The api of the resource to lookup.
    /// @param model The model of the resource to lookup.
    /// @return a `shared_ptr` to the resource's registration data.
    std::shared_ptr<const ModelRegistration> lookup_model(const API& api, const Model& model) const;

    /// @brief Register a resource client constructor
    template <typename ResourceClientT>
    void register_resource_client() {
        class ResourceClientRegistration2 final : public ResourceClientRegistration {
           public:
            using ResourceClientRegistration::ResourceClientRegistration;

            std::shared_ptr<Resource> create_rpc_client(
                std::string name, std::shared_ptr<GrpcChannel> chan) const override {
                return std::make_shared<ResourceClientT>(std::move(name), std::move(chan));
            }
        };

        Registry::register_resource_client_(API::get<typename ResourceClientT::interface_type>(),
                                            std::make_shared<ResourceClientRegistration2>());
    }

    /// @brief Register a resource server constructor.
    template <typename ResourceServerT>
    void register_resource_server() {
        class ResourceServerRegistration2 final : public ResourceServerRegistration {
           public:
            using ResourceServerRegistration::ResourceServerRegistration;
            std::shared_ptr<ResourceServer> create_resource_server(
                std::shared_ptr<ResourceManager> manager, Server& server) const override {
                auto rs = std::make_shared<ResourceServerT>(manager);
                server.register_service(rs.get());
                return rs;
            }
        };

        const google::protobuf::ServiceDescriptor* sd =
            get_service_descriptor_(ResourceServerT::service_type::service_full_name());
        Registry::register_resource_server_(API::get<typename ResourceServerT::interface_type>(),
                                            std::make_shared<ResourceServerRegistration2>(sd));
    }

    /// @brief Register resource client and server constructors
    template <typename ResourceClientT, typename ResourceServerT>
    void register_resource() {
        register_resource_client<ResourceClientT>();
        register_resource_server<ResourceServerT>();
    }

    /// @brief Lookup a registered server api.
    /// @param api The api to lookup.
    /// @return A `shared_ptr` to the registered api's `ResourceServerRegistration`.
    std::shared_ptr<const ResourceServerRegistration> lookup_resource_server(const API& api) const;

    /// @brief Lookup a registered client api.
    /// @param api The api to lookup.
    /// @return A `shared_ptr` to the registered api's `ResourceClientRegistration`.
    std::shared_ptr<const ResourceClientRegistration> lookup_resource_client(const API& api) const;

    /// @brief Provide information on registered resource models.
    /// @return A map from name to `ModelRegistration` of all registered resource models.
    const std::unordered_map<std::string, std::shared_ptr<const ModelRegistration>>&
    registered_models() const;

    /// @brief Provide access to registered resources.
    /// @return A map from `API` to `ResourceServerRegistration` of all registered resources.
    const std::unordered_map<API, std::shared_ptr<const ResourceServerRegistration>>&
    registered_resource_servers() const;

   private:
    friend class Instance;
    Registry() = default;

    /// @brief Initialized the Viam registry. No-op if it has already been called.
    void initialize();

    mutable std::mutex lock_;

    std::unordered_map<std::string, std::shared_ptr<const ModelRegistration>> resources_;

    std::unordered_map<API, std::shared_ptr<const ResourceClientRegistration>> client_apis_;
    std::unordered_map<API, std::shared_ptr<const ResourceServerRegistration>> server_apis_;

    void register_resources();

    void register_resource_server_(
        API api, std::shared_ptr<ResourceServerRegistration> resource_registration);

    void register_resource_client_(
        API api, std::shared_ptr<ResourceClientRegistration> resource_registration);

    static const google::protobuf::ServiceDescriptor* get_service_descriptor_(
        const char* service_full_name);

    std::shared_ptr<const ModelRegistration> lookup_model_inlock_(
        const std::string& name, const std::lock_guard<std::mutex>&) const;
};

}  // namespace sdk
}  // namespace viam
