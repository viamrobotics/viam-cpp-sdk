#include <google/protobuf/descriptor.h>

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

       public:
	std::string to_string();
	Subtype(std::string namespace_, std::string resource_type,
		std::string resource_subtype);
	Subtype(Type type, std::string resource_subtype);
};

class Name : public Subtype {
	std::string remote_name;
	std::string name;

       public:
	std::string to_string();
	friend bool operator==(Name& lhs, Name& rhs);
};

class RPCSubtype {
       public:
	Subtype subtype;
	std::string proto_service_name;
	google::protobuf::Descriptor descriptor;
	// CR erodkin: are these other operators necessary?
	size_t operator()(RPCSubtype const& key) const {
		Subtype subtype = key.subtype;
		std::string hash = subtype.to_string() + proto_service_name;
		return std::hash<std::string>()(hash);
	}

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
	size_t operator()(Name const& key) const noexcept;
};

template <>
struct std::hash<RPCSubtype> {
	size_t operator()(RPCSubtype const& key) const noexcept;
};
