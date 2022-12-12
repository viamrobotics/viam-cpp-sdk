#include <string>

#include "common/v1/common.pb.h"
using viam::common::v1::ResourceName;
class ComponentType {
       public:
	std::string name;
};

class ComponentBase {
       public:
	std::string name;
	ComponentType type;

	// TODO (RSDK-921): implement this
	ResourceName get_resource_name(std::string name);
};

