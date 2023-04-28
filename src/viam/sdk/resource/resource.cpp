#include <viam/sdk/resource/resource.hpp>

#include <regex>
#include <sstream>
#include <string>

#include <boost/algorithm/string.hpp>
#include <google/protobuf/descriptor.h>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/component_base.hpp>

namespace viam {
namespace sdk {

const std::regex NAME_REGEX("^([\\w-]+:[\\w-]+:(?:[\\w-]+))\\/?([\\w-]+:(?:[\\w-]+:)*)?(.+)?$");

const std::regex MODEL_REGEX("^([\\w-]+):([\\w-]+):([\\w-]+)$");
std::regex SINGLE_FIELD_REGEX("^([\\w-]+)$");

std::string Type::to_string() const {
    return namespace_ + ":" + resource_type_;
}

Type::Type(std::string namespace_, std::string resource_type)
    : namespace_(namespace_), resource_type_(resource_type){};

std::string Subtype::to_string() const {
    return Type::to_string() + ":" + resource_subtype_;
}

const std::string& Type::type_namespace() const {
    return namespace_;
}

void Type::set_namespace(const std::string type_namespace) {
    this->namespace_ = type_namespace;
}

void Type::set_resource_type(const std::string resource_type) {
    this->resource_type_ = resource_type;
}

const std::string& Type::resource_type() const {
    return resource_type_;
}

const std::string& Subtype::resource_subtype() const {
    return resource_subtype_;
}

void Subtype::set_resource_subtype(const std::string subtype) {
    this->resource_subtype_ = subtype;
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
    : Type(type), resource_subtype_(resource_subtype) {}

Subtype::Subtype(std::string namespace_, std::string resource_type, std::string resource_subtype)
    : Type(namespace_, resource_type), resource_subtype_(resource_subtype) {}

bool Subtype::is_service_type() {
    return (this->resource_type() == "service");
}

bool Subtype::is_component_type() {
    return (this->resource_type() == "component");
}

const Subtype* Name::to_subtype() const {
    return this;
}

const std::string& Name::name() const {
    return name_;
}

const std::string& Name::remote_name() const {
    return remote_name_;
}

std::string Name::to_string() const {
    std::string subtype_name = Subtype::to_string();
    if (remote_name_ == "") {
        return subtype_name + "/" + name_;
    }
    return subtype_name + "/" + remote_name_ + ":" + name_;
}

const std::string Name::short_name() const {
    if (remote_name_ != "") {
        return remote_name_ + ":" + name_;
    }
    return name_;
}

const viam::common::v1::ResourceName Name::to_proto() const {
    viam::common::v1::ResourceName rn;
    *rn.mutable_namespace_() = this->type_namespace();
    *rn.mutable_name() = this->name();
    *rn.mutable_type() = this->resource_type();
    *rn.mutable_subtype() = this->resource_subtype();
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
    : Subtype(subtype), remote_name_(std::move(remote)), name_(std::move(name)) {}

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
    return lhs.subtype_.to_string() == rhs.subtype_.to_string() &&
           lhs.proto_service_name_ == rhs.proto_service_name_ &&
           lhs.descriptor_.DebugString() == rhs.descriptor_.DebugString();
}

bool operator==(const Model& lhs, const Model& rhs) {
    return lhs.to_string() == rhs.to_string();
}

RPCSubtype::RPCSubtype(Subtype subtype,
                       std::string proto_service_name,
                       const google::protobuf::ServiceDescriptor& descriptor)
    : descriptor_(std::move(descriptor)),
      proto_service_name_(std::move(proto_service_name)),
      subtype_(std::move(subtype)) {}

RPCSubtype::RPCSubtype(Subtype subtype, const google::protobuf::ServiceDescriptor& descriptor)
    : descriptor_(std::move(descriptor)), subtype_(std::move(subtype)) {}

const std::string& RPCSubtype::proto_service_name() const {
    return proto_service_name_;
};

const Subtype& RPCSubtype::subtype() const {
    return subtype_;
};

ModelFamily::ModelFamily(std::string namespace_, std::string family)
    : namespace_(namespace_), family_(family) {}

Model::Model(ModelFamily model_family, std::string model_name)
    : model_family_(std::move(model_family)), model_name_(std::move(model_name)) {}

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
        return family_;
    }
    return namespace_ + ":" + family_;
}

std::string Model::to_string() const {
    const std::string mf = model_family_.to_string();
    if (mf == "") {
        return model_name_;
    }
    return mf + ":" + model_name_;
}

Model::Model() : Model(ModelFamily(RDK, BUILTIN), BUILTIN) {}

}  // namespace sdk
}  // namespace viam
