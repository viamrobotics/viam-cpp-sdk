#include <google/protobuf/struct.pb.h>
#include <grpcpp/support/status.h>

#include <string>

#include "common/v1/common.pb.h"
using viam::common::v1::ResourceName;
class ComponentType {
       public:
	std::string name;
	friend bool operator==(ComponentType& lhs, ComponentType& rhs);
};

class ComponentBase {
       public:
	std::string name;
	ComponentType type;
	std::vector<ComponentType> component_hierarchy;
	ResourceName get_resource_name(std::string name);
	virtual grpc::StatusCode stop(
	    google::protobuf::Map<std::string, google::protobuf::Value> extra);
	virtual grpc::StatusCode stop();
};

bool operator==(ComponentType& lhs, ComponentType& rhs) {
	return lhs.name == rhs.name;
}

// CR erodkin: consider having a version of this that takes no argument and
// assumes this.name
ResourceName ComponentBase::get_resource_name(std::string name_) {
	// CR erodkin: we don't have mro or split on viam.components in here,
	// unlike python.
	ResourceName r;
	*r.mutable_namespace_() = "rdk";
	*r.mutable_type() = "component";
	*r.mutable_subtype() = name;
	*r.mutable_name() = name_;

	return r;
}

grpc::StatusCode ComponentBase::stop(
    google::protobuf::Map<std::string, google::protobuf::Value> ex) {
	return stop();
}

grpc::StatusCode ComponentBase::stop() {
	return grpc::StatusCode::UNIMPLEMENTED;
}
