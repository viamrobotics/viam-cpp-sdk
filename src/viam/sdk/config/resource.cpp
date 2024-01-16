#include <viam/sdk/config/resource.hpp>

#include <numeric>
#include <string>
#include <unordered_map>

#include <boost/algorithm/string.hpp>

#include <viam/api/app/v1/robot.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/referenceframe/frame.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

Name ResourceConfig::resource_name() {
    try {
        this->fix_api();
    } catch (std::string err) {  // NOLINT
        throw err;
    }
    std::vector<std::string> remotes;
    // NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDeleteLeaks)
    boost::split(remotes, this->name_, boost::is_any_of(":"));
    if (remotes.size() > 1) {
        const std::string str_name(remotes.back());
        remotes.pop_back();
        const std::string remote =
            std::accumulate(remotes.begin(), remotes.end(), std::string(":"));
        return Name(this->api_, remote, str_name);
    }
    return Name(this->api_, "", remotes.at(0));
}

void ResourceConfig::set_api(API api) {
    api_ = std::move(api);
}

const API& ResourceConfig::api() const {
    return api_;
}

void ResourceConfig::set_frame(LinkConfig frame) {
    frame_ = std::move(frame);
}

const LinkConfig& ResourceConfig::frame() const {
    return frame_;
}

void ResourceConfig::set_model(Model model) {
    model_ = std::move(model);
}

const Model& ResourceConfig::model() const {
    return model_;
}

void ResourceConfig::set_name(std::string name) {
    name_ = std::move(name);
}

const std::string& ResourceConfig::name() const {
    return name_;
}

void ResourceConfig::set_namespace(std::string ns) {
    namespace__ = std::move(ns);
}

const std::string& ResourceConfig::namespace_() const {
    return namespace__;
}

const std::string& ResourceConfig::type() const {
    return type_;
}

const AttributeMap& ResourceConfig::attributes() const {
    return attributes_;
}

void ResourceConfig::fix_api() {
    if (this->api_.type_namespace().empty() && this->namespace__.empty()) {
        this->namespace__ = kRDK;
        this->api_.set_namespace(kRDK);
    } else if (this->api_.type_namespace().empty()) {
        this->api_.set_namespace(this->namespace__);
    } else {
        this->namespace__ = this->api_.type_namespace();
    }

    if (this->api_.resource_type().empty()) {
        this->api_.set_resource_type(kComponent);
    }

    if (this->api_.resource_subtype().empty()) {
        this->api_.set_resource_subtype(this->type_);
    } else if (this->type_.empty()) {
        this->type_ = this->api_.resource_subtype();
    }

    // This shouldn't be able to happen except with directly instantiated
    // config structs
    if (this->api_.type_namespace() != this->namespace__ ||
        this->api_.resource_subtype() != this->type_) {
        throw "component namespace and/or type do not match component api field";
    }
}

ResourceConfig ResourceConfig::from_proto(viam::app::v1::ComponentConfig proto_cfg) {
    ResourceConfig resource(proto_cfg.type());
    resource.name_ = proto_cfg.name();
    resource.namespace__ = proto_cfg.namespace_();
    resource.type_ = proto_cfg.type();
    resource.attributes_ = struct_to_map(proto_cfg.attributes());
    const std::string& api = proto_cfg.api();
    if (api.find(':') != std::string::npos) {
        resource.api_ = API::from_string(api);
    }
    resource.model_ = Model::from_str(proto_cfg.model());

    try {
        resource.fix_api();
    } catch (std::string err) {  // NOLINT
        throw err;
    }

    if (proto_cfg.has_frame()) {
        resource.frame_ = LinkConfig::from_proto(proto_cfg.frame());
    }

    return resource;
};

viam::app::v1::ComponentConfig ResourceConfig::to_proto() const {
    viam::app::v1::ComponentConfig proto_cfg;
    const google::protobuf::Struct s = map_to_struct(attributes_);
    const google::protobuf::RepeatedPtrField<viam::app::v1::ResourceLevelServiceConfig>
        service_configs;

    for (const auto& svc_cfg : service_config_) {
        viam::app::v1::ResourceLevelServiceConfig cfg;
        *cfg.mutable_type() = svc_cfg.type;
        *cfg.mutable_attributes() = map_to_struct(svc_cfg.attributes);
        *proto_cfg.mutable_service_configs()->Add() = cfg;
    }

    *proto_cfg.mutable_name() = name_;
    *proto_cfg.mutable_namespace_() = namespace__;
    *proto_cfg.mutable_type() = type_;
    *proto_cfg.mutable_api() = api_.to_string();
    const std::string mm = model_.to_string();
    *proto_cfg.mutable_model() = mm;
    *proto_cfg.mutable_attributes() = map_to_struct(attributes_);
    for (const auto& dep : depends_on_) {
        *proto_cfg.mutable_depends_on()->Add() = dep;
    }
    *proto_cfg.mutable_frame() = frame_.to_proto();

    return proto_cfg;
}

ResourceConfig::ResourceConfig(std::string type) : api_({kRDK, type, ""}), type_(type){};

}  // namespace sdk
}  // namespace viam
