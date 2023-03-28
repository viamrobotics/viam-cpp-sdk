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

Type::Type(std::string namespace_, std::string resource_type)
    : namespace_(namespace_), resource_type(resource_type){};

std::string Subtype::to_string() const {
    return Type::to_string() + ":" + resource_subtype;
}

Subtype Subtype::from_string(std::string subtype) {
    if (std::regex_match(subtype, MODEL_REGEX)) {
        std::vector<std::string> subtype_parts;
        boost::split(subtype_parts, subtype, boost::is_any_of(":"));
        return {subtype_parts.at(0), subtype_parts.at(1), subtype_parts.at(2)};
    } else {
        throw "string " + subtype + " is not a valid subtype name";
    }
}

Subtype::Subtype(Type type, std::string resource_subtype)
    : Type(type), resource_subtype(resource_subtype) {}

Subtype::Subtype(std::string namespace_, std::string resource_type, std::string resource_subtype)
    : Type(namespace_, resource_type), resource_subtype(resource_subtype) {}

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

Name Name::from_string(std::string name) {
    if (!std::regex_match(name, NAME_REGEX)) {
        throw "Received invalid Name string: " + name;
    }
    std::vector<std::string> slash_splits;
    boost::split(slash_splits, name, boost::is_any_of("/"));

    Subtype subtype = Subtype::from_string(slash_splits.at(0));

    std::vector<std::string> colon_splits;
    boost::split(colon_splits, slash_splits.at(1), boost::is_any_of(":"));
    std::string remote = "";
    std::string resource_name = colon_splits.at(0);
    if (colon_splits.size() > 1) {
        remote = colon_splits.at(0);
        resource_name = colon_splits.at(1);
    }

    return Name(subtype, remote, resource_name);
}

Name::Name(Subtype subtype, std::string remote, std::string name)
    : Subtype(subtype), remote_name(std::move(remote)), name(std::move(name)) {}

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
                       const google::protobuf::ServiceDescriptor& descriptor)
    : subtype(subtype), descriptor(&descriptor), proto_service_name(proto_service_name) {}

RPCSubtype::RPCSubtype(Subtype subtype, const google::protobuf::ServiceDescriptor& descriptor)
    : subtype(subtype), descriptor(&descriptor) {}

ModelFamily::ModelFamily(std::string namespace_, std::string family)
    : namespace_(namespace_), family(family) {}

Model::Model(ModelFamily model_family, std::string model_name)
    : model_family(std::move(model_family)), model_name(std::move(model_name)) {}

Model::Model(std::string namespace_, std::string family, std::string model_name)
    : Model(ModelFamily(std::move(namespace_), std::move(family)), std::move(model_name)) {}

Model Model::from_str(std::string model) {
    if (std::regex_match(model, MODEL_REGEX)) {
        std::vector<std::string> model_parts;
        boost::split(model_parts, model, boost::is_any_of(":"));
        return {model_parts.at(0), model_parts.at(1), model_parts.at(2)};
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
