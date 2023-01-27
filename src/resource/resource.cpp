#include <common/v1/common.pb.h>
#include <google/protobuf/descriptor.h>

#include <boost/algorithm/string.hpp>
#include <components/component_base.hpp>
#include <regex>
#include <sstream>
#include <string>

class Type {
   public:
    std::string namespace_;
    std::string resource_type;
    std::string to_string() const;
};

class Subtype : public Type {
   public:
    std::string resource_subtype;
    std::string to_string() const;
    Subtype(std::string subtype);
    Subtype(Type type, std::string resource_subtype);
    Subtype(std::string namespace_, std::string resource_type, std::string resource_subtype);
    friend bool operator==(Subtype& lhs, Subtype& rhs);
    Subtype();
    bool is_component_type();
    bool is_service_type();
};

class Name : public Subtype {
   public:
    std::string remote;
    std::string name;

    const std::string to_string() const;
    const Subtype* to_subtype() const;
    Name(std::string name);
    Name(Subtype subtype, std::string remote, std::string name);
    viam::common::v1::ResourceName to_proto();
};

class RPCSubtype {
   public:
    Subtype subtype;
    std::string proto_service_name;
    google::protobuf::ServiceDescriptor& descriptor;
    RPCSubtype(Subtype subtype, google::protobuf::ServiceDescriptor& descriptor);
    RPCSubtype(Subtype subtype,
               std::string proto_service_name,
               google::protobuf::ServiceDescriptor& descriptor);

    friend bool operator==(const RPCSubtype& lhs, const RPCSubtype& rhs);
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
    Model(std::string namespace_, std::string family, std::string model_name);
    Model(ModelFamily model, std::string model_name);
    Model(std::string model);
    std::string to_string();
    friend bool operator==(const Model& lhs, const Model& rhs);
};

const std::regex NAME_REGEX("^([\\w-]+:[\\w-]+:(?:[\\w-]+))\\/?([\\w-]+:(?:[\\w-]+:)*)?(.+)?$");

const std::regex MODEL_REGEX("^([\\w-]+):([\\w-]+):([\\w-]+)$");
std::regex SINGLE_FIELD_REGEX("^([\\w-]+)$");

template <>
struct std::hash<Name> {
    size_t operator()(Name const& key) const noexcept {
        std::string hash =
            key.namespace_ + key.resource_type + key.resource_subtype + key.name + key.remote;
        return std::hash<std::string>()(hash);
    }
};

template <>
struct std::hash<Subtype> {
    size_t operator()(const Subtype& key) const noexcept {
        std::string const hash = key.to_string();
        return std::hash<std::string>()(hash);
    }
};

template <>
struct std::hash<RPCSubtype> {
    size_t operator()(RPCSubtype const& key) const noexcept {
        Subtype subtype = key.subtype;
        std::string hash =
            subtype.to_string() + key.proto_service_name + key.descriptor.DebugString();

        return std::hash<std::string>()(hash);
    }
};

std::string Type::to_string() const {
    return namespace_ + ":" + resource_type;
}

std::string Subtype::to_string() const {
    return Type::to_string() + ":" + resource_subtype;
}

Subtype::Subtype(std::string subtype) {
    if (std::regex_match(subtype, MODEL_REGEX)) {
        std::vector<std::string> subtype_parts;
        boost::split(subtype_parts, subtype, boost::is_any_of(":"));
        this->namespace_ = subtype_parts.at(0);
        this->resource_type = subtype_parts.at(1);
        this->resource_subtype = subtype_parts.at(2);
    } else {
        throw "string " + subtype + " is not a valid subtype name";
    }
}

Subtype::Subtype(Type type, std::string resource_subtype) {
    namespace_ = type.namespace_;
    resource_type = type.resource_type;
    resource_subtype = resource_subtype;
}

Subtype::Subtype(std::string namespace_, std::string resource_type, std::string resource_subtype) {
    namespace_ = namespace_;
    resource_type = resource_type;
    resource_subtype = resource_subtype;
}

bool Subtype::is_service_type() {
    return (this->resource_type == "service");
}

bool Subtype::is_component_type() {
    return (this->resource_type == "component");
}

const Subtype* Name::to_subtype() const {
    return this;
}

const std::string Name::to_string() const {
    std::string subtype_name = Subtype::to_string();
    if (remote == "") {
        return subtype_name + "/" + name;
    }
    return subtype_name + "/" + remote + ":" + name;
}

viam::common::v1::ResourceName Name::to_proto() {
    viam::common::v1::ResourceName rn;
    *rn.mutable_namespace_() = this->namespace_;
    *rn.mutable_name() = this->name;
    *rn.mutable_type() = this->resource_type;
    *rn.mutable_subtype() = this->resource_subtype;
    return rn;
}

Name::Name(std::string name) {
    if (!std::regex_match(name, NAME_REGEX)) {
        throw "Received invalid Name string: " + name;
    }
    std::vector<std::string> matches;
    boost::split(matches, name, boost::is_any_of(":"));

    std::vector<std::string> subtype_parts;
    boost::split(subtype_parts, matches.at(1), boost::is_any_of(":"));

    std::string remote = matches.at(2);
    if (remote.size() > 0) {
        remote.pop_back();
    }

    this->remote = remote;
    this->namespace_ = subtype_parts.at(0);
    this->resource_type = subtype_parts.at(1);
    this->resource_subtype = subtype_parts.at(2);
    this->name = matches.at(3);
}

Name::Name(Subtype subtype, std::string remote, std::string name) {
    this->remote = remote;
    this->name = name;
    this->resource_subtype = subtype.resource_subtype;
    this->namespace_ = subtype.namespace_;
    this->resource_type = subtype.resource_type;
}

bool operator==(Subtype& lhs, Subtype& rhs) {
    return lhs.to_string() == rhs.to_string();
}
bool operator==(Name& lhs, Name& rhs) {
    return lhs.to_string() == rhs.to_string();
}

bool operator==(RPCSubtype& lhs, RPCSubtype& rhs) {
    return lhs.subtype.to_string() == rhs.subtype.to_string() &&
           lhs.proto_service_name == rhs.proto_service_name &&
           lhs.descriptor.DebugString() == rhs.descriptor.DebugString();
}

bool operator==(Model& lhs, Model& rhs) {
    return lhs.to_string() == rhs.to_string();
}

RPCSubtype::RPCSubtype(Subtype subtype,
                       std::string proto_service_name,
                       google::protobuf::ServiceDescriptor& descriptor)
    : subtype(subtype), descriptor(descriptor) {
    proto_service_name = proto_service_name;
}

RPCSubtype::RPCSubtype(Subtype subtype, google::protobuf::ServiceDescriptor& descriptor)
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

Model::Model(std::string namespace_, std::string family, std::string model_name) {
    namespace_ = namespace_;
    family = family;
    model_name = model_name;
}

// Parses a single model string into a Model. If only a model_name is
// included, then default values will be used for namespace and family.
//
// Raises:
// 	Will throw an error if an invalid model string is passed (i.e.,
// using non-word characters)
Model::Model(std::string model) {
    if (std::regex_match(model, MODEL_REGEX)) {
        std::vector<std::string> model_parts;
        boost::split(model_parts, model, boost::is_any_of(":"));
        namespace_ = model_parts.at(0);
        family = model_parts.at(1);
        model_name = model_parts.at(2);
    } else if (std::regex_match(model, SINGLE_FIELD_REGEX)) {
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
