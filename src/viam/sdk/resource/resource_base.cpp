#include <viam/sdk/resource/resource_base.hpp>

#include <unordered_map>

#include <grpcpp/support/status.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

grpc::StatusCode ResourceBase::stop(AttributeMap extra) {
    return stop();
}

grpc::StatusCode ResourceBase::stop() {
    return grpc::StatusCode::UNIMPLEMENTED;
}

std::string ResourceBase::name() const {
    return name_;
}

Subtype ResourceBase::subtype() {
    return {RDK, RESOURCE, "ResourceBase"};
}

void ResourceBase::reconfigure(Dependencies deps, Resource cfg){};

ResourceType ResourceBase::type() const {
    return {RESOURCE};
}

ResourceName ResourceBase::get_resource_name(std::string name) {
    ResourceName r;
    *r.mutable_namespace_() = RDK;
    *r.mutable_type() = RESOURCE;
    *r.mutable_subtype() = this->instance_subtype().resource_subtype();
    *r.mutable_name() = std::move(name);

    return r;
}

}  // namespace sdk
}  // namespace viam
