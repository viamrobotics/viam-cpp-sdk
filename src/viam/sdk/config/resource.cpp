#include <viam/sdk/config/resource.hpp>

#include <numeric>
#include <string>
#include <unordered_map>

#include <boost/algorithm/string.hpp>

#include <viam/api/app/v1/robot.pb.h>
#include <viam/api/robot/v1/robot.pb.h>
#include <viam/api/service/discovery/v1/discovery.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/private/repeated_ptr_convert.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/referenceframe/frame.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

ResourceConfig::ResourceConfig(std::string type,
                               std::string name,
                               std::string namespace_,
                               ProtoStruct attributes,
                               std::string api,
                               Model model,
                               LinkConfig frame,
                               log_level lvl)
    : api_({kRDK, type, ""}),
      frame_(std::move(frame)),
      model_(std::move(model)),
      name_(std::move(name)),
      namespace__(std::move(namespace_)),
      type_(std::move(type)),
      attributes_(std::move(attributes)),
      log_level_(lvl) {
    if (api.find(':') != std::string::npos) {
        api_ = API::from_string(std::move(api));
    }
    fix_api();
}

ResourceConfig::ResourceConfig(std::string type) : api_({kRDK, type, ""}), type_(std::move(type)) {}

Name ResourceConfig::resource_name() {
    this->fix_api();
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

const API& ResourceConfig::api() const {
    return api_;
}

const LinkConfig& ResourceConfig::frame() const {
    return frame_;
}

const Model& ResourceConfig::model() const {
    return model_;
}

const std::string& ResourceConfig::name() const {
    return name_;
}

const std::string& ResourceConfig::namespace_() const {
    return namespace__;
}

const std::string& ResourceConfig::type() const {
    return type_;
}

log_level ResourceConfig::get_log_level() const {
    return log_level_;
}

const std::vector<std::string>& viam::sdk::ResourceConfig::depends_on() const {
    return depends_on_;
}

const std::vector<ResourceLevelServiceConfig>& viam::sdk::ResourceConfig::service_config() const {
    return service_config_;
}

const ProtoStruct& ResourceConfig::attributes() const {
    return attributes_;
}

void ResourceConfig::fix_api() {
    if (this->api_.type_namespace().empty() && this->namespace__.empty()) {
        this->namespace__ = kRDK;
        this->api_.namespace_ = kRDK;
    } else if (this->api_.type_namespace().empty()) {
        api_.namespace_ = namespace__;
    } else {
        this->namespace__ = this->api_.type_namespace();
    }

    if (this->api_.resource_type().empty()) {
        api_.resource_type_ = kComponent;
    }

    if (this->api_.resource_subtype().empty()) {
        api_.resource_subtype_ = this->type_;
    } else if (this->type_.empty()) {
        this->type_ = this->api_.resource_subtype();
    }

    // This shouldn't be able to happen except with directly instantiated
    // config structs
    if (this->api_.type_namespace() != this->namespace__ ||
        this->api_.resource_subtype() != this->type_) {
        throw Exception("component namespace and/or type do not match component api field");
    }
}

namespace proto_convert_details {

void to_proto_impl<ResourceLevelServiceConfig>::operator()(
    const ResourceLevelServiceConfig& self, app::v1::ResourceLevelServiceConfig* proto) const {
    *proto->mutable_type() = self.type;
    *proto->mutable_attributes() = to_proto(self.attributes);
}

void to_proto_impl<ResourceConfig>::operator()(const ResourceConfig& self,
                                               app::v1::ComponentConfig* proto) const {
    *proto->mutable_service_configs() = impl::to_repeated_field(self.service_config());

    *proto->mutable_name() = self.name();
    *proto->mutable_namespace_() = self.namespace_();
    *proto->mutable_type() = self.type();
    *proto->mutable_api() = self.api().to_string();
    *proto->mutable_log_configuration()->mutable_level() = to_string(self.get_log_level());
    *proto->mutable_model() = self.model().to_string();
    *proto->mutable_attributes() = to_proto(self.attributes());

    *proto->mutable_depends_on() = ::google::protobuf::RepeatedPtrField<std::string>(
        self.depends_on().begin(), self.depends_on().end());

    *proto->mutable_frame() = to_proto(self.frame());
}

ResourceConfig from_proto_impl<app::v1::ComponentConfig>::operator()(
    const app::v1::ComponentConfig* proto) const {
    const std::string& level_str = proto->log_configuration().level();
    return ResourceConfig(proto->type(),
                          proto->name(),
                          proto->namespace_(),
                          from_proto(proto->attributes()),
                          proto->api(),
                          Model::from_str(proto->model()),
                          proto->has_frame() ? from_proto(proto->frame()) : LinkConfig{},
                          level_str.empty() ? log_level::info : level_from_string(level_str));
}

std::vector<ResourceConfig>
from_proto_impl<service::discovery::v1::DiscoverResourcesResponse>::operator()(
    const service::discovery::v1::DiscoverResourcesResponse* proto) const {
    return impl::from_repeated_field(proto->discoveries());
}

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
