#include <viam/sdk/resource/resource_api.hpp>

#include <numeric>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

#include <boost/algorithm/string.hpp>
#include <google/protobuf/descriptor.h>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/component.hpp>

namespace viam {
namespace sdk {

// NOLINTNEXTLINE
const std::regex NAME_REGEX("^([\\w-]+:[\\w-]+:(?:[\\w-]+))\\/?([\\w-]+:(?:[\\w-]+:)*)?(.+)?$");

// NOLINTNEXTLINE
const std::regex MODEL_REGEX("^([\\w-]+):([\\w-]+):([\\w-]+)$");

// NOLINTNEXTLINE
std::regex SINGLE_FIELD_REGEX("^([\\w-]+)$");

std::string APIType::to_string() const {
    return namespace_ + ":" + resource_type_;
}

APIType::APIType(std::string namespace_, std::string resource_type)
    : namespace_(std::move(namespace_)), resource_type_(std::move(resource_type)) {}

std::string API::to_string() const {
    return APIType::to_string() + ":" + resource_subtype_;
}

const std::string& APIType::type_namespace() const {
    return namespace_;
}

void APIType::set_namespace(const std::string& type_namespace) {
    this->namespace_ = type_namespace;
}

void APIType::set_resource_type(const std::string& resource_type) {
    this->resource_type_ = resource_type;
}

const std::string& APIType::resource_type() const {
    return resource_type_;
}

const std::string& API::resource_subtype() const {
    return resource_subtype_;
}

void API::set_resource_subtype(const std::string& subtype) {
    this->resource_subtype_ = subtype;
}

API API::from_string(std::string api) {
    if (std::regex_match(api, MODEL_REGEX)) {
        std::vector<std::string> api_parts;
        boost::split(api_parts, api, boost::is_any_of(":"));
        return {api_parts.at(0), api_parts.at(1), api_parts.at(2)};
    }
    throw Exception("string " + api + " is not a valid api name");
}

API::API(APIType type, std::string resource_subtype)
    : APIType(std::move(type)), resource_subtype_(std::move(resource_subtype)) {}

API::API(std::string namespace_, std::string resource_type, std::string resource_subtype)
    : APIType(std::move(namespace_), std::move(resource_type)),
      resource_subtype_(std::move(resource_subtype)) {}

bool API::is_service_type() {
    return (this->resource_type() == "service");
}

bool API::is_component_type() {
    return (this->resource_type() == "component");
}

Name::Name(API api, std::string remote, std::string name)
    : api_(std::move(api)), remote_name_(std::move(remote)), name_(std::move(name)) {}

const API& Name::api() const {
    return api_;
}

const std::string& Name::name() const {
    return name_;
}

const std::string& Name::remote_name() const {
    return remote_name_;
}

std::string Name::to_string() const {
    if (remote_name_.empty() || remote_name_ == ":") {
        return api_.to_string() + "/" + name_;
    }
    return api_.to_string() + "/" + remote_name_ + ":" + name_;
}

std::string Name::short_name() const {
    if (!remote_name_.empty()) {
        return remote_name_ + ":" + name_;
    }
    return name_;
}

Name Name::from_string(std::string name) {
    if (!std::regex_match(name, NAME_REGEX)) {
        throw Exception("Received invalid Name string: " + name);
    }
    std::vector<std::string> slash_splits;
    boost::split(slash_splits, name, boost::is_any_of("/"));

    const API api = API::from_string(slash_splits.at(0));

    std::vector<std::string> colon_splits;
    boost::split(colon_splits, slash_splits.at(1), boost::is_any_of(":"));
    std::string remote;
    std::string resource_name = colon_splits.at(0);
    if (colon_splits.size() > 1) {
        remote = colon_splits.at(0);
        resource_name = colon_splits.at(1);
    }

    return Name(api, remote, resource_name);
}

namespace proto_convert_details {

void to_proto<Name>::operator()(const Name& self, common::v1::ResourceName* proto) const {
    *proto->mutable_namespace_() = self.api().type_namespace();
    if (self.remote_name().empty()) {
        *proto->mutable_name() = self.name();
    } else {
        *proto->mutable_name() = self.remote_name() + ":" + self.name();
    }
    *proto->mutable_type() = self.api().resource_type();
    *proto->mutable_subtype() = self.api().resource_subtype();
}

Name from_proto<common::v1::ResourceName>::operator()(const common::v1::ResourceName* proto) const {
    auto name_parts = long_name_to_remote_and_short(proto->name());

    return Name({proto->namespace_(), proto->type(), proto->subtype()},
                name_parts.first,
                name_parts.second);
}

}  // namespace proto_convert_details

bool operator==(const API& lhs, const API& rhs) {
    return lhs.to_string() == rhs.to_string();
}

bool operator<(const API& lhs, const API& rhs) {
    return lhs.to_string() < rhs.to_string();
}

std::ostream& operator<<(std::ostream& os, const API& v) {
    os << v.to_string();
    return os;
}

bool operator==(const Name& lhs, const Name& rhs) {
    return lhs.to_string() == rhs.to_string();
}

std::ostream& operator<<(std::ostream& os, const Name& v) {
    os << v.to_string();
    return os;
}

bool operator==(const RPCSubtype& lhs, const RPCSubtype& rhs) {
    return lhs.api_.to_string() == rhs.api_.to_string() &&
           lhs.proto_service_name_ == rhs.proto_service_name_ &&
           lhs.descriptor_.DebugString() == rhs.descriptor_.DebugString();
}

bool operator==(const Model& lhs, const Model& rhs) {
    return lhs.to_string() == rhs.to_string();
}

RPCSubtype::RPCSubtype(API api,
                       std::string proto_service_name,
                       const google::protobuf::ServiceDescriptor& descriptor)
    : descriptor_(descriptor),
      proto_service_name_(std::move(proto_service_name)),
      api_(std::move(api)) {}

RPCSubtype::RPCSubtype(API api, const google::protobuf::ServiceDescriptor& descriptor)
    : descriptor_(descriptor), api_(std::move(api)) {}

const std::string& RPCSubtype::proto_service_name() const {
    return proto_service_name_;
};

const API& RPCSubtype::api() const {
    return api_;
};

ModelFamily::ModelFamily(std::string namespace_, std::string family)
    : namespace_(std::move(namespace_)), family_(std::move(family)) {}

Model::Model(ModelFamily model_family, std::string model_name)
    : model_family_(std::move(model_family)), model_name_(std::move(model_name)) {}

Model::Model(std::string namespace_, std::string family, std::string model_name)
    : Model(ModelFamily(std::move(namespace_), std::move(family)), std::move(model_name)) {}

Model Model::from_str(std::string model) {
    if (std::regex_match(model, MODEL_REGEX)) {
        std::vector<std::string> model_parts;
        boost::split(model_parts, model, boost::is_any_of(":"));
        return {model_parts.at(0), model_parts.at(1), model_parts.at(2)};
    }
    if (std::regex_match(model, SINGLE_FIELD_REGEX)) {
        return {"rdk", "builtin", model};
    }
    throw Exception("string " + model + " is not a valid model name");
}

std::string ModelFamily::to_string() const {
    if (namespace_.empty()) {
        return family_;
    }
    return namespace_ + ":" + family_;
}

std::string Model::to_string() const {
    const std::string mf = model_family_.to_string();
    if (mf.empty()) {
        return model_name_;
    }
    return mf + ":" + model_name_;
}

Model::Model() : Model(ModelFamily(kRDK, kBuiltin), kBuiltin) {}

}  // namespace sdk
}  // namespace viam
