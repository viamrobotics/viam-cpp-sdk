#pragma once
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>

#include <common/proto_type.hpp>
#include <functional>
#include <resource/resource.hpp>
#include <unordered_map>

class ResourceBase;
using Dependencies = std::unordered_map<Name, std::shared_ptr<ResourceBase>>;
class ResourceBase : public grpc::Service {
   public:
    virtual grpc::StatusCode stop(std::unordered_map<std::string, ProtoType*> extra);
    virtual grpc::StatusCode stop();
    std::function<void(Dependencies)> reconfigure;
};
