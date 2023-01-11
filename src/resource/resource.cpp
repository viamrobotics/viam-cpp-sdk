#include <google/protobuf/descriptor.h>

#include <boost/algorithm/string.hpp>
#include <regex>
#include <sstream>
#include <string>

class Type {
       public:
	std::string namespace_;
	std::string resource_type;
	std::string to_string();
};

class Subtype : public Type {
       public:
	std::string resource_subtype;
	std::string to_string();
	Subtype(Type type, std::string resource_subtype);
	Subtype(std::string namespace_, std::string resource_type,
		std::string resource_subtype);
};

class Name : public Subtype {
       public:
	std::string remote;
	std::string name;

	std::string const to_string();
};

class RPCSubtype {
       public:
	Subtype subtype;
	std::string proto_service_name;
	// CR erodkin: should this be ServiceDescriptor? looks like it is in Go,
	// but ServiceDescriptor doesn't exist for a handler in cpp (see
	// module/client.cc)
	const google::protobuf::Descriptor& descriptor;
	RPCSubtype(Subtype subtype,
		   const google::protobuf::Descriptor& descriptor);
	RPCSubtype(Subtype subtype, std::string proto_service_name,
		   google::protobuf::Descriptor& descriptor);

	friend bool operator==(RPCSubtype& lhs, RPCSubtype& rhs);
};

class ModelFamily {
       public:
	std::string namespace_;
	std::string family;
	ModelFamily(std::string namespace_, std::string family);
	ModelFamily();
	std::string to_string();
};

class Model : public ModelFamily {
       public:
	std::string model_name;
	Model(std::string namespace_, std::string family,
	      std::string model_name);
	Model(ModelFamily model, std::string model_name);
	Model(std::string model);
	std::string to_string();
};

template <>
struct std::hash<Name> {
	size_t operator()(Name const& key) const noexcept {
		std::string hash = key.namespace_ + key.resource_type +
				   key.resource_subtype + key.name + key.remote;
		return std::hash<std::string>()(hash);
	}
};

template <>
struct std::hash<RPCSubtype> {
	size_t operator()(RPCSubtype const& key) const noexcept {
		Subtype subtype = key.subtype;
		std::string hash = subtype.to_string() +
				   key.proto_service_name +
				   key.descriptor.DebugString();

		return std::hash<std::string>()(hash);
	}
};

std::string Type::to_string() { return namespace_ + ":" + resource_type; }

std::string Subtype::to_string() {
	return Type::to_string() + ":" + resource_subtype;
}

std::string Name::to_string() {
	std::string subtype_name = Subtype::to_string();
	if (remote == "") {
		return subtype_name + "/" + name;
	}
	return subtype_name + "/" + remote + ":" + name;
}

bool operator==(Name& lhs, Name& rhs) {
	return lhs.to_string() == rhs.to_string();
}

bool operator==(RPCSubtype& lhs, RPCSubtype& rhs) {
	return lhs.subtype.to_string() == rhs.subtype.to_string() &&
	       lhs.proto_service_name == rhs.proto_service_name &&
	       lhs.descriptor.DebugString() == rhs.descriptor.DebugString();
}

Subtype::Subtype(Type type, std::string resource_subtype) {
	namespace_ = type.namespace_;
	resource_type = type.resource_type;
	resource_subtype = resource_subtype;
}

Subtype::Subtype(std::string namespace_, std::string resource_type,
		 std::string resource_subtype) {
	namespace_ = namespace_;
	resource_type = resource_type;
	resource_subtype = resource_subtype;
}

RPCSubtype::RPCSubtype(Subtype subtype, std::string proto_service_name,
		       google::protobuf::Descriptor& descriptor)
    : subtype(subtype), descriptor(descriptor) {
	proto_service_name = proto_service_name;
}

RPCSubtype::RPCSubtype(Subtype subtype,
		       const google::protobuf::Descriptor& descriptor)
    : subtype(subtype), descriptor(descriptor) {}

ModelFamily::ModelFamily(std::string namespace_, std::string family) {
	namespace_ = namespace_;
	family = family;
}

Model::Model(ModelFamily model_family, std::string model_name) {
	model_name = model_name;
	namespace_ = model_family.namespace_;
	family = model_family.family;
}

Model::Model(std::string namespace_, std::string family,
	     std::string model_name) {
	namespace_ = namespace_;
	family = family;
	model_name = model_name;
}

// Parses a single model string into a Model. If only a model_name is included,
// then default values will be used for namespace and family.
//
// Raises:
// 	Will throw an error if an invalid model string is passed (i.e., using
// non-word characters)
Model::Model(std::string model) {
	std::regex model_regex(
	    "^([a-zA-Z_0-9-]+):([a-zA-Z_0-9-]+):([a-zA-Z_0-9-]+)$");
	std::regex single_field_regex("^([a-zA-Z_0-9-]+)$");

	if (std::regex_match(model, model_regex)) {
		std::vector<std::string> model_parts;
		boost::split(model_parts, model, boost::is_any_of(":"));
		namespace_ = model_parts.at(0);
		family = model_parts.at(1);
		model_name = model_parts.at(2);
	} else if (std::regex_match(model, single_field_regex)) {
		namespace_ = "rdk";
		family = "builtin";
		model_name = model;
	} else {
		throw "string " + model + " is not a valid model name";
	}
}

std::string ModelFamily::to_string() {
	if (namespace_ == "") {
		return family;
	}
	return namespace_ + ":" + family;
}

std::string Model::to_string() {
	std::string mf = ModelFamily::to_string();
	if (mf == "") {
		return model_name;
	}
	return mf + ":" + model_name;
}
