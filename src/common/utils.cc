#include <boost/blank.hpp>
#include <boost/optional/optional.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../components/component_base.h"
#include "../registry/registry.h"
#include "common/v1/common.pb.h"

using viam::common::v1::ResourceName;

std::vector<ResourceName> resource_names_for_component(
    ComponentBase component) {
	std::string* component_type;
	std::vector<ResourceName> resource_names;
	for (auto a : Registry::registered_components()) {
		ComponentRegistration reg = a.second;
		if (reg.component_type == component.type) {
			component_type = &reg.name;
		}
	}

	if (component_type == nullptr) {
		component_type = &component.name;
	}

	// TODO (RSDK-1631): make sure we split properly on "viam.components."
	// within component_type as needed

	ResourceName r;
	*r.mutable_namespace_() = "rdk";
	*r.mutable_type() = "component";
	*r.mutable_name() = component.name;
	*r.mutable_subtype() = *component_type;
	resource_names.push_back(r);
	std::cout << "called to get resource names! " << r.SerializeAsString()
		  << std::endl;
	return resource_names;
}

class ResourceNameHasher {
	size_t operator()(ResourceName const& key) const {
		return std::hash<std::string>()(key.SerializeAsString());
	}
};

class ResourceNameEqual {
	bool operator()(ResourceName const& t1, ResourceName const& t2) const {
		return !(
		    t1.SerializeAsString().compare(t2.SerializeAsString()));
	}

	static bool check_equal(ResourceName const& r1,
				ResourceName const& r2) {
		return r1.SerializeAsString().compare(r2.SerializeAsString());
	}
};
