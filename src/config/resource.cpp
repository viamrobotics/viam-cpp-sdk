#include <app/v1/robot.pb.h>
#include <robot/v1/robot.pb.h>

#include <boost/algorithm/string.hpp>
#include <common/proto_type.hpp>
#include <common/utils.hpp>
#include <config/resource.hpp>
#include <numeric>
#include <referenceframe/frame.hpp>
#include <resource/resource.hpp>
#include <string>
#include <unordered_map>

typedef std::unordered_map<std::string, ProtoType*> AttributeMap;

Name Component::resource_name() {
    try {
        this->fix_api();
    } catch (std::string err) {
        throw err;
    }
    std::vector<std::string> remotes;
    boost::split(remotes, this->name, boost::is_any_of(":"));
    if (remotes.size() > 1) {
        std::string str_name = remotes.at(remotes.size() - 1);
        remotes.pop_back();
        std::string remote = std::accumulate(remotes.begin(), remotes.end(), ":");
        return Name(this->api, remote, str_name);
    }
    return Name(this->api, "", remotes.at(0));
}

void Component::fix_api() {
    if (this->api.namespace_ == "" && this->namespace_ == "") {
        this->namespace_ = RDK;
        this->api.namespace_ = RDK;
    } else if (this->api.namespace_ == "") {
        this->api.namespace_ = this->namespace_;
    } else {
        this->namespace_ = this->api.namespace_;
    }

    if (this->api.resource_type == "") {
        this->api.resource_type = COMPONENT;
    }

    if (this->api.resource_subtype == "") {
        this->api.resource_subtype = this->type;
    } else if (this->type == "") {
        this->type = this->api.resource_subtype;
    }

    // This shouldn't be able to happen except with directly instantiated
    // config structs
    if (this->api.namespace_ != this->namespace_ || this->api.resource_subtype != this->type) {
        throw "component namespace and/or type do not match component api field";
    }
}

Component Component::from_proto(viam::app::v1::ComponentConfig proto_cfg) {
    Component component;
    component.name = proto_cfg.name();
    component.namespace_ = proto_cfg.namespace_();
    component.type = proto_cfg.type();
    std::string api = proto_cfg.api();
    if (api.find(":") != std::string::npos) {
        component.api = Subtype(api);
    }

    try {
        component.fix_api();
    } catch (std::string err) {
        throw err;
    }

    if (proto_cfg.has_frame()) {
        LinkConfig lc = LinkConfig::from_proto(proto_cfg.frame());
    }

    return component;
};

viam::app::v1::ComponentConfig Component::to_proto() {
    viam::app::v1::ComponentConfig proto_cfg;
    google::protobuf::Struct s = map_to_struct(attributes);
    google::protobuf::RepeatedPtrField<viam::app::v1::ResourceLevelServiceConfig> service_configs;

    for (auto& svc_cfg : service_config) {
        viam::app::v1::ResourceLevelServiceConfig cfg;
        *cfg.mutable_type() = svc_cfg.type;
        *cfg.mutable_attributes() = map_to_struct(svc_cfg.attributes);
        *proto_cfg.mutable_service_configs()->Add() = cfg;
    }

    *proto_cfg.mutable_name() = name;
    *proto_cfg.mutable_namespace_() = namespace_;
    *proto_cfg.mutable_type() = type;
    *proto_cfg.mutable_api() = api.to_string();
    *proto_cfg.mutable_model() = model.to_string();
    *proto_cfg.mutable_attributes() = map_to_struct(attributes);
    for (auto& dep : depends_on) {
        *proto_cfg.mutable_depends_on()->Add() = dep;
    }
    *proto_cfg.mutable_frame() = frame.to_proto();

    return proto_cfg;
}

Service Service::from_component_config(Component cfg) {
    Service svc;
    svc.name = cfg.name;
    svc.namespace_ = cfg.namespace_;
    svc.type = cfg.type;
    svc.model = cfg.model;
    svc.depends_on = cfg.depends_on;
    svc.attributes = cfg.attributes;
    svc.converted_attributes = cfg.converted_attributes;
    svc.implicit_depends_on = cfg.implicit_depends_on;
    return svc;
}

