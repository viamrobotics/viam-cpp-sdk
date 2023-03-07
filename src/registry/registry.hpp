#pragma once

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
#include <string>
#include <subtype/subtype.hpp>

// TODO(RSDK-1742): instead of std::functions, consider making these functions virtual
// TODO(RSDK-1742): one class per header
class ResourceSubtype {
   public:
    std::function<ResourceBase(ResourceBase, Name)> create_reconfigurable;
    std::function<ProtoType(ResourceBase)> create_status;
    const google::protobuf::ServiceDescriptor* service_descriptor;
    virtual std::shared_ptr<ResourceServerBase> create_resource_server(
        std::shared_ptr<SubtypeService> svc);
    std::function<std::shared_ptr<ResourceBase>(std::string, std::shared_ptr<grpc::Channel>)>
        create_rpc_client;

    ResourceSubtype(const google::protobuf::ServiceDescriptor* service_descriptor)
        : service_descriptor(service_descriptor){};

   private:
};

class ModelRegistration {
   public:
    ModelRegistration(ResourceType rt) : resource_type(std::move(rt)){};
    ModelRegistration(
        ResourceType rt,
        Subtype subtype,
        Model model,
        std::function<std::shared_ptr<ResourceBase>(Dependencies, Resource)> constructor)
        : subtype(std::move(subtype)),
          resource_type(std::move(rt)),
          model(std::move(model)),
          construct_resource(std::move(constructor)){};
    Subtype subtype;
    Model model;
    ResourceType resource_type;
    std::function<std::shared_ptr<ResourceBase>(Dependencies, Resource)> construct_resource;
    viam::robot::v1::Status create_status(std::shared_ptr<ResourceBase> resource);
};

class Registry {
   public:
    /// Registers a resource with the Registry
    /// Args:
    /// 	resource (ModelRegistration): object containing resource registration data
    ///
    /// Raises:
    /// 	throws error if resource already exists in the registry
    static void register_resource(std::shared_ptr<ModelRegistration> resource);
    static std::shared_ptr<ModelRegistration> lookup_resource(std::string name);
    static std::shared_ptr<ModelRegistration> lookup_resource(Subtype subtype, Model model);
    static void register_subtype(Subtype subtype,
                                 std::shared_ptr<ResourceSubtype> resource_subtype);
    static std::shared_ptr<ResourceSubtype> lookup_subtype(Subtype subtype);
    static std::unordered_map<std::string, std::shared_ptr<ModelRegistration>>
    registered_resources();

   private:
    static std::unordered_map<std::string, std::shared_ptr<ModelRegistration>> resources;
    static std::unordered_map<Subtype, std::shared_ptr<ResourceSubtype>> subtypes;
};

