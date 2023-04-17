#include <resource/resource_base.hpp>

#include <unordered_map>

#include <grpcpp/support/status.h>

#include <common/proto_type.hpp>
#include <common/utils.hpp>
#include <resource/resource.hpp>

namespace viam {
namespace cppsdk {

grpc::StatusCode ResourceBase::stop(AttributeMap extra) {
    return stop();
}

grpc::StatusCode ResourceBase::stop() {
    return grpc::StatusCode::UNIMPLEMENTED;
}

std::string ResourceBase::name() {
    return name_;
}

Subtype ResourceBase::subtype() {
    return {RDK, "resource", "ResourceBase"};
}

void ResourceBase::reconfigure(Dependencies deps, Resource cfg){};

ResourceType ResourceBase::type() {
    return {"resource"};
}

ResourceName ResourceBase::get_resource_name(std::string name) {
    ResourceName r;
    *r.mutable_namespace_() = RDK;
    *r.mutable_type() = "resource";
    *r.mutable_subtype() = this->type().to_string();
    *r.mutable_name() = std::move(name);

    return r;
}

}  // namespace cppsdk
}  // namespace viam
