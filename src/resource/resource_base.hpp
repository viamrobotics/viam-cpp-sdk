#pragma once

#include <unordered_map>

#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>

#include <common/proto_type.hpp>
#include <config/resource.hpp>
#include <resource/resource.hpp>
#include <resource/resource_type.hpp>

class ResourceBase;
using Dependencies = std::unordered_map<Name, std::shared_ptr<ResourceBase>>;
class ResourceBase {
   public:
    ResourceBase(ResourceType type) : type(std::move(type)){};

    std::string name;
    ResourceType type;
    virtual grpc::StatusCode stop(std::unordered_map<std::string, ProtoType*> extra);
    virtual grpc::StatusCode stop();
    virtual void reconfigure(Dependencies deps, Resource cfg);
};
