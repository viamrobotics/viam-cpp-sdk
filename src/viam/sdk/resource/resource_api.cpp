#include <viam/sdk/resource/resource_api.hpp>

#include <numeric>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

#include <boost/algorithm/string.hpp>
#include <google/protobuf/descriptor.h>

#include <viam/api/common/v1/common.pb.h>

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
    : namespace_(namespace_), resource_type_(resource_type){};

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
    throw "string " + api + " is not a valid api name";
}

API::API(APIType type, std::string resource_subtype)
    : APIType(type), resource_subtype_(resource_subtype) {}

API::API(std::string namespace_, std::string resource_type, std::string resource_subtype)
    : APIType(namespace_, resource_type), resource_subtype_(resource_subtype) {}

bool API::is_service_type() {
    return (this->resource_type() == "service");
}

bool API::is_component_type() {
    return (this->resource_type() == "component");
}

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

viam::common::v1::ResourceName Name::to_proto() const {
    viam::common::v1::ResourceName rn;
    *rn.mutable_namespace_() = this->api().type_namespace();
    *rn.mutable_name() = this->name();
    *rn.mutable_type() = this->api().resource_type();
    *rn.mutable_subtype() = this->api().resource_subtype();
    return rn;
}

Name Name::from_proto(const viam::common::v1::ResourceName& proto) {
    const API api(proto.namespace_(), proto.type(), proto.subtype());
    std::vector<std::string> name_parts;
    boost::split(name_parts, proto.name(), boost::is_any_of(":"));
    auto name = name_parts.back();
    name_parts.pop_back();
    auto remote_name = std::accumulate(name_parts.begin(), name_parts.end(), std::string(":"));

    return Name({proto.namespace_(), proto.type(), proto.subtype()}, remote_name, name);
};

Name Name::from_string(std::string name) {
    if (!std::regex_match(name, NAME_REGEX)) {
        throw "Received invalid Name string: " + name;
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

Name::Name(API api, std::string remote, std::string name)
    : api_(api), remote_name_(std::move(remote)), name_(std::move(name)) {}

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
    : descriptor_(std::move(descriptor)),
      proto_service_name_(std::move(proto_service_name)),
      api_(std::move(api)) {}

RPCSubtype::RPCSubtype(API api, const google::protobuf::ServiceDescriptor& descriptor)
    : descriptor_(std::move(descriptor)), api_(std::move(api)) {}

const std::string& RPCSubtype::proto_service_name() const {
    return proto_service_name_;
};

const API& RPCSubtype::api() const {
    return api_;
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
    }
    if (std::regex_match(model, SINGLE_FIELD_REGEX)) {
        return {"rdk", "builtin", model};
    }
    throw std::runtime_error("string " + model + " is not a valid model name");
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
