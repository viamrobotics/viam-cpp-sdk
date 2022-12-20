#include <boost/blank.hpp>
#include <boost/optional/optional.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <tuple>
#include <unordered_map>

#include "../components/component_base.h"
#include "../registry/registry.h"
#include "common/v1/common.pb.h"
#include "google/protobuf/map.h"
#include "google/protobuf/struct.pb.h"

using google::protobuf::Struct;
using google::protobuf::Value;
using viam::common::v1::ResourceName;

// float, string, struct, array, struct(map from string to any of the above)
class ProtoType {
       public:
	boost::variant<boost::blank, bool, std::string, int, double> proto_type;
	ProtoType(std::string s) { proto_type = s; }
	ProtoType(bool b) { proto_type = b; }
	ProtoType(int i) { proto_type = i; }

	Value proto_value() {
		Value v;
		switch (proto_type.which()) {
			case 0: {
				break;
			}
			case 1: {
				const bool b = boost::get<bool>(proto_type);
				v.set_bool_value(b);
				break;
			}
			case 2: {
				const std::string s =
				    boost::get<std::string>(proto_type);
				*v.mutable_string_value() = s;
				break;
			}
			case 3: {
				const int i = boost::get<int>(proto_type);
				v.set_number_value(i);
				break;
			}
			case 4: {
				const double i = boost::get<double>(proto_type);
				v.set_number_value(i);
				break;
			}
			default: {
				throw "Invalid proto_value conversion type. This should never happen;\
						please file a bug report.";
			}
		}

		return v;
	}
};

google::protobuf::Value proto_value_of_prototype(std::string p) {
	google::protobuf::Value v;
	*(v.mutable_string_value()) = p;
	return v;
}

Struct map_to_struct(std::unordered_map<std::string, ProtoType> dict) {
	Struct s;
	for (auto& key_and_value : dict) {
		const std::string key = key_and_value.first;
		Value value = key_and_value.second.proto_value();
		google::protobuf::MapPair<std::string, Value> val(key, value);
		s.mutable_fields()->insert(val);
	}

	return s;
}

// CR erodkin: hum. why doesn't this work?
std::unordered_map<std::string, Value> struct_to_map(Struct struct_) {
	return struct_.mutable_fields();
}

std::vector<ResourceName> resource_names_for_component(
    ComponentBase component) {
	// CR erodkin: split on viam.components here?
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
	ResourceName r;
	*r.mutable_namespace_() = "rdk";
	*r.mutable_type() = "component";
	*r.mutable_name() = component.name;
	*r.mutable_subtype() = *component_type;
	resource_names.push_back(r);
	return resource_names;
}
