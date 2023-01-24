#pragma once

#include <app/v1/robot.pb.h>
#include <common/proto_type.h>
#include <referenceframe/frame.h>
#include <resource/resource.h>
#include <robot/v1/robot.pb.h>

#include <string>
#include <unordered_map>

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
	Subtype api;
	Model model;
	LinkConfig frame;
	std::vector<std::string> depends_on;
	std::vector<ResourceLevelServiceConfig> service_config;
	AttributeMap attributes;
	ProtoType converted_attributes;
	std::vector<std::string> implicit_depends_on;

	static Component from_proto(viam::app::v1::ComponentConfig proto_cfg);
	viam::app::v1::ComponentConfig to_proto();
	Component();
	Name resource_name();

       private:
	void fix_api();
};

