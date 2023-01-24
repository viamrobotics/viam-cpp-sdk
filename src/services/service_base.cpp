#include <resource/resource_base.hpp>
#include <string>

class ServiceType {
	std::string name;
	friend bool operator==(ServiceType& lhs, ServiceType& rhs);
	ServiceType(std::string name) { name = name; }
	ServiceType() { name = "ServiceBase"; }
};

class ServiceBase : public ResourceBase {
	// CR erodkin: do we need more? see ComponentBase for model
	ServiceType type;
	std::string name;
};

