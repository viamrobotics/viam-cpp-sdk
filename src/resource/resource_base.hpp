#pragma once

#include <unordered_map>

#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>

#include <common/proto_type.hpp>
#include <config/resource.hpp>
#include <resource/resource.hpp>
#include <resource/resource_type.hpp>

namespace viam {
namespace cppsdk {

class ResourceBase;
using Dependencies = std::unordered_map<Name, std::shared_ptr<ResourceBase>>;
class ResourceBase {
   public:
    explicit ResourceBase(std::string name) : name_(std::move(name)){};
    static Subtype subtype();
    virtual grpc::StatusCode stop(std::unordered_map<std::string, ProtoType*> extra);
    virtual grpc::StatusCode stop();
    virtual void reconfigure(Dependencies deps, Resource cfg);
    virtual std::string name();
    virtual ResourceType type();

   private:
    std::string name_;
};

}  // namespace cppsdk
}  // namespace viam
