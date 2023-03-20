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
    GenericSubtype(const google::protobuf::ServiceDescriptor* service_descriptor)
        : ResourceSubtype(service_descriptor){};
};

class Generic : public ComponentBase {
   public:
    static std::shared_ptr<ResourceSubtype> resource_subtype();
    static Subtype subtype();
    virtual std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> do_command(std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> command) = 0;
};

