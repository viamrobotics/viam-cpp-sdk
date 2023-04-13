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

#include <robot/v1/robot.pb.h>

#include <config/resource.hpp>
#include <resource/resource.hpp>
#include <resource/resource_base.hpp>
#include <resource/resource_server_base.hpp>
#include <resource/resource_type.hpp>
#include <subtype/subtype.hpp>

// TODO(RSDK-1742): instead of std::functions, consider making these functions
// virtual
// TODO(RSDK-1742): one class per header
/// @class ResourceSubtype registry.hpp "registry/registry.hpp"
/// @brief Defines registered `Resource` creation functionality.
class ResourceSubtype {
   public:
    /// @brief Add `Reconfigure` functionality to a resource.
    std::function<ResourceBase(ResourceBase, Name)> create_reconfigurable;

    // TODO: it doesn't look like we actually use this. Confirm, then remove.
    std::function<ProtoType(ResourceBase)> create_status;

    /// @brief Create a resource's gRPC server.
    /// @param svc The server's `SubtypeService`.
    /// @return a `shared_ptr` to the gRPC server.
    virtual std::shared_ptr<ResourceServerBase> create_resource_server(
        std::shared_ptr<SubtypeService> svc) = 0;

    /// @brief Create gRPC client to a resource.
    /// @param name The name of the resource.
    /// @param channel A channel connected to the client.
    /// @return A `shared_ptr` to the resource client.
    virtual std::shared_ptr<ResourceBase> create_rpc_client(
        std::string name, std::shared_ptr<grpc::Channel> channel) = 0;

    ResourceSubtype(const google::protobuf::ServiceDescriptor* service_descriptor)
        : service_descriptor(service_descriptor){};

   private:
    const google::protobuf::ServiceDescriptor* service_descriptor;
};

/// @class ModelRegistration
/// @brief Information about a registered model, including a constructor and config validator.
class ModelRegistration {
   public:
    ModelRegistration(ResourceType rt)
        : resource_type(std::move(rt)), validate(default_validator){};
    ModelRegistration(
        ResourceType rt,
        Subtype subtype,
        Model model,
        std::function<std::shared_ptr<ResourceBase>(Dependencies, Resource)> constructor)
        : subtype(std::move(subtype)),
          resource_type(std::move(rt)),
          model(std::move(model)),
          construct_resource(std::move(constructor)),
          validate(default_validator){};
    ModelRegistration(
        ResourceType rt,
        Subtype subtype,
        Model model,
        std::function<std::shared_ptr<ResourceBase>(Dependencies, Resource)> constructor,
        std::function<std::vector<std::string>(Resource)> validator)
        : subtype(std::move(subtype)),
          resource_type(std::move(rt)),
          model(std::move(model)),
          construct_resource(std::move(constructor)),
          validate(std::move(validator)){};
    Subtype subtype;
    Model model;
    ResourceType resource_type;

    /// @brief Constructs a resource from a map of dependencies and a resource config.
    std::function<std::shared_ptr<ResourceBase>(Dependencies, Resource)> construct_resource;

    /// @brief Validates a resource config.
    std::function<std::vector<std::string>(Resource)> validate;

    /// @brief Creates a `Status` object for a given resource.
    viam::robot::v1::Status create_status(std::shared_ptr<ResourceBase> resource);

   private:
    // default_validator is the default validator for all models if no validator
    // is provided in construction. No dependencies are returned.
    static const std::vector<std::string> default_validator(Resource cfg) {
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
    static void register_resource(std::shared_ptr<ModelRegistration> resource);

    /// @brief Lookup a given registered resource.
    /// @param name The name of the resource to lookup.
    /// @return a `shared_ptr` to the resource's registration data.
    static std::shared_ptr<ModelRegistration> lookup_resource(std::string name);

    /// @brief Lookup a given registered resource.
    /// @param subtype The subtype of the resource to lookup.
    /// @param model The model of the resource to lookup.
    /// @return a `shared_ptr` to the resource's registration data.
    static std::shared_ptr<ModelRegistration> lookup_resource(Subtype subtype, Model model);

    /// @brief Register a subtype.
    /// @param subtype The subtype to be registered.
    /// @param resource_subtype `ResourceSubtype` with resource functionality.
    static void register_subtype(Subtype subtype,
                                 std::shared_ptr<ResourceSubtype> resource_subtype);

    /// @brief Lookup a registered subtype.
    /// @param subtype The subtype to lookup.
    /// @return A `shared_ptr` to the registered subtype's `ResourceSubtype`.
    static std::shared_ptr<ResourceSubtype> lookup_subtype(Subtype subtype);

    /// @brief Provide information on registered resources.
    /// @return A map from name to `ModelRegistration` of all registered resources.
    static std::unordered_map<std::string, std::shared_ptr<ModelRegistration>>
    registered_resources();

   private:
    static std::unordered_map<std::string, std::shared_ptr<ModelRegistration>> resources;
    static std::unordered_map<Subtype, std::shared_ptr<ResourceSubtype>> subtypes;
};
