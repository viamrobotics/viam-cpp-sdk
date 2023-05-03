#pragma once

#include <unordered_map>

#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_type.hpp>

namespace viam {
namespace sdk {

class Resource;
using Dependencies = std::unordered_map<Name, std::shared_ptr<Resource>>;
class Resource {
   public:
    virtual ~Resource();
    explicit Resource(std::string name) : name_(std::move(name)){};
    static Subtype subtype();
    virtual viam::common::v1::ResourceName get_resource_name(std::string name);
    virtual grpc::StatusCode stop(AttributeMap extra);
    virtual grpc::StatusCode stop();
    virtual void reconfigure(Dependencies deps, ResourceConfig cfg);
    virtual std::string name();
    virtual ResourceType type();

   private:
    std::string name_;
};

}  // namespace sdk
}  // namespace viam
