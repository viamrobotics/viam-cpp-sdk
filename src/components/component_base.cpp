#include <google/protobuf/struct.pb.h>
#include <grpcpp/support/status.h>

#include <string>

#include "../common/proto_type.hpp"
#include "common/v1/common.pb.h"
#include "component_base.hpp"

using viam::common::v1::ResourceName;

ComponentBase::ComponentBase() {}

ComponentType ComponentBase::type = ComponentType();

bool operator==(ComponentType& lhs, ComponentType& rhs) {
    return lhs.name == rhs.name;
}

ResourceName ComponentBase::get_resource_name(std::string name_) {
    // TODO (RSDK-1631): test, confirm whether we need to split on
    // "viam.components" here
    ResourceName r;
    *r.mutable_namespace_() = "rdk";
    *r.mutable_type() = "component";
    *r.mutable_subtype() = type.name;
    *r.mutable_name() = name_;
    std::cout << "calling get_resource_name! " << r.SerializeAsString() << std::endl;

    return r;
}

grpc::StatusCode ComponentBase::stop(std::unordered_map<std::string, ProtoType> ex) {
    return stop();
}

grpc::StatusCode ComponentBase::stop() {
    return grpc::StatusCode::UNIMPLEMENTED;
}
