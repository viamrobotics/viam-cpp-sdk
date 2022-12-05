#ifndef COMPONENT_BASE_H
#define COMPONENT_BASE_H

#include <string>

#include "common/v1/common.pb.h"

class ComponentType {
       public:
	std::string name;
};

class ComponentBase {
       public:
	std::string name;
	ComponentType type;

	// TODO (RSDK-921): implement this
	viam::common::v1::ResourceName get_resource_name(std::string name);
};

#endif
