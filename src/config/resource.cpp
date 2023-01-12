#include <common/proto_type.h>
#include <referenceframe/frame.h>
#include <resource/resource.h>
#include <robot/v1/robot.pb.h>

#include <string>
#include <unordered_map>

#include "app/v1/robot.pb.h"

typedef std::unordered_map<std::string, ProtoType*> AttributeMap;

class ResourceLevelServiceConfig {
       public:
	std::string type;
	AttributeMap attributes;
	ProtoType converted_attributes;
};

class Component {
       public:
	std::string name;
	std::string namespace_;
	std::string type;
	viam::robot::v1::ResourceRPCSubtype api;
	Model model;
	LinkConfig frame;
	std::vector<std::string> depends_on;
	std::vector<ResourceLevelServiceConfig> service_config;
	AttributeMap attributes;
	ProtoType converted_attributes;
	std::vector<std::string> implicit_depends_on;

	static Component from_proto(viam::app::v1::ComponentConfig proto_cfg);
	viam::app::v1::ComponentConfig to_proto();
};

Component Component::from_proto(viam::app::v1::ComponentConfig proto_cfg){};

viam::app::v1::ComponentConfig Component::to_proto() {
	viam::app::v1::ComponentConfig proto_cfg;
	google::protobuf::Struct s = map_to_struct(attributes);
	google::protobuf::RepeatedPtrField<
	    viam::app::v1::ResourceLevelServiceConfig>
	    service_configs;

	for (auto svc_cfg : service_config) {
		viam::app::v1::ResourceLevelServiceConfig cfg;
		*cfg.mutable_type() = svc_cfg.type;
		*cfg.mutable_attributes() = map_to_struct(svc_cfg.attributes);
		*proto_cfg.mutable_service_configs()->Add() = cfg;
	}

	*proto_cfg.mutable_name() = name;
	*proto_cfg.mutable_namespace_() = namespace_;
	*proto_cfg.mutable_type() = type;
	*proto_cfg.mutable_api() = api.SerializeAsString();
	*proto_cfg.mutable_model() = model.to_string();
	*proto_cfg.mutable_attributes() = map_to_struct(attributes);
	for (auto dep : depends_on) {
		*proto_cfg.mutable_depends_on()->Add() = dep;
	}
	*proto_cfg.mutable_frame() = frame.to_proto();

	return proto_cfg;
}
