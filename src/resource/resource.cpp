#include <common/v1/common.pb.h>
#include <google/protobuf/descriptor.h>

#include <boost/algorithm/string.hpp>
#include <common/utils.hpp>
#include <components/component_base.hpp>
#include <regex>
#include <resource/resource.hpp>
#include <sstream>
#include <string>

const std::regex NAME_REGEX("^([\\w-]+:[\\w-]+:(?:[\\w-]+))\\/?([\\w-]+:(?:[\\w-]+:)*)?(.+)?$");

const std::regex MODEL_REGEX("^([\\w-]+):([\\w-]+):([\\w-]+)$");
std::regex SINGLE_FIELD_REGEX("^([\\w-]+)$");

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

std::string Name::to_string() const {
    std::string subtype_name = Subtype::to_string();
    if (remote_name == "") {
        return subtype_name + "/" + name;
    }
    return subtype_name + "/" + remote_name + ":" + name;
}

std::string Name::short_name() const {
    if (remote_name == "") {
        return remote_name + ":" + name;
    }
    return name;
}

viam::common::v1::ResourceName Name::to_proto() {
    viam::common::v1::ResourceName rn;
    *rn.mutable_namespace_() = this->namespace_;
    *rn.mutable_name() = this->name;
    *rn.mutable_type() = this->resource_type;
    *rn.mutable_subtype() = this->resource_subtype;
    return rn;
}

Name::Name(std::string name) : Subtype("") {
    if (!std::regex_match(name, NAME_REGEX)) {
        throw "Received invalid Name string: " + this->name;
    }
    std::vector<std::string> matches;
    boost::split(matches, name, boost::is_any_of(":"));

    std::vector<std::string> subtype_parts;
    boost::split(subtype_parts, matches.at(1), boost::is_any_of(":"));

    std::string remote = matches.at(2);
    if (remote.size() > 0) {
        remote.pop_back();
    }

    this->remote_name = remote;
    this->namespace_ = subtype_parts.at(0);
    this->resource_type = subtype_parts.at(1);
    this->resource_subtype = subtype_parts.at(2);
    this->name = matches.at(3);
}

Name::Name(Subtype subtype, std::string remote, std::string name) : Subtype("") {
    this->remote_name = remote;
    this->name = name;
    this->resource_subtype = subtype.resource_subtype;
    this->namespace_ = subtype.namespace_;
    this->resource_type = subtype.resource_type;
}

bool operator==(const Subtype& lhs, const Subtype& rhs) {
    return lhs.to_string() == rhs.to_string();
}

bool operator<(const Subtype& lhs, const Subtype& rhs) {
    return lhs.to_string() < rhs.to_string();
}

bool operator==(const Name& lhs, const Name& rhs) {
    return lhs.to_string() == rhs.to_string();
}

bool operator==(const RPCSubtype& lhs, const RPCSubtype& rhs) {
    return lhs.subtype.to_string() == rhs.subtype.to_string() &&
           lhs.proto_service_name == rhs.proto_service_name &&
           lhs.descriptor->DebugString() == rhs.descriptor->DebugString();
}

bool operator==(const Model& lhs, const Model& rhs) {
    return lhs.to_string() == rhs.to_string();
}

RPCSubtype::RPCSubtype(Subtype subtype,
                       std::string proto_service_name,
                       google::protobuf::Descriptor& descriptor)
    : subtype(subtype), descriptor(&descriptor) {
    proto_service_name = proto_service_name;
}

RPCSubtype::RPCSubtype(Subtype subtype, const google::protobuf::Descriptor& descriptor)
    : subtype(subtype), descriptor(&descriptor) {}

ModelFamily::ModelFamily(std::string namespace_, std::string family) {
    namespace_ = namespace_;
    family = family;
}

Model::Model(ModelFamily model_family, std::string model_name)
    : model_family(std::move(model_family)), model_name(std::move(model_name)) {}

Model::Model(std::string namespace_, std::string family, std::string model_name)
    : Model(ModelFamily(std::move(namespace_), std::move(family)), std::move(model_name)) {}

// Parses a single model string into a Model. If only a model_name is
// included, then default values will be used for namespace and family.
//
// Raises:
// 	Will throw an error if an invalid model string is passed (i.e.,
// using non-word characters)
Model Model::from_str(std::string model) {
    if (std::regex_match(model, MODEL_REGEX)) {
        std::vector<std::string> model_parts;
        boost::split(model_parts, model, boost::is_any_of(":"));
        std::string namespace_ = model_parts.at(0);
        std::string family = model_parts.at(1);
        std::string model_name = model_parts.at(2);
        return {namespace_, family, model_name};
    } else if (std::regex_match(model, SINGLE_FIELD_REGEX)) {
        return {"rdk", "builtin", model};
    } else {
        throw "string " + model + " is not a valid model name";
    }
}

std::string ModelFamily::to_string() const {
    if (namespace_ == "") {
        return family;
    }
    return namespace_ + ":" + family;
}

std::string Model::to_string() const {
    const std::string mf = model_family.to_string();
    if (mf == "") {
        return model_name;
    }
    return mf + ":" + model_name;
}

Model::Model() : Model(ModelFamily(RDK, BUILTIN), BUILTIN) {}

// CR erodkin: delete these
Model foo() {
    ModelFamily mf{"my namespace", "my family"};
    Model m{mf, "my name"};
    return m;
}

Model bar() {
    return {{"my namespace", "my family"}, "my name"};
}
