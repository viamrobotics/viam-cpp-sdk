#pragma once

#include <common/utils.hpp>
#include <registry/registry.hpp>
#include <resource/resource.hpp>

#include "common/proto_type.hpp"
#include "subtype/subtype.hpp"

// TODO(RSDK-1742): one class per header
class GenericSubtype : public ResourceSubtype {
   public:
    std::shared_ptr<ResourceServerBase> create_resource_server(
        std::shared_ptr<SubtypeService> svc) override;
    std::shared_ptr<ResourceBase> create_rpc_client(std::string name, std::shared_ptr<grpc::Channel> chan) override;
    GenericSubtype(const google::protobuf::ServiceDescriptor* service_descriptor)
        : ResourceSubtype(service_descriptor){};
};

class Generic : public ComponentBase {
   public:
    static std::shared_ptr<ResourceSubtype> resource_subtype();
    static Subtype subtype();
    virtual ProtoType do_command(std::unordered_map<std::string, ProtoType*> command) = 0;
};

