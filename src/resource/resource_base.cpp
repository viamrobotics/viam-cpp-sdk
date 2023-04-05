#include <resource/resource_base.hpp>

#include <unordered_map>

#include <grpcpp/support/status.h>

#include <common/proto_type.hpp>
#include <common/utils.hpp>
#include <resource/resource.hpp>

grpc::StatusCode ResourceBase::stop(std::unordered_map<std::string, ProtoType*> extra) {
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
