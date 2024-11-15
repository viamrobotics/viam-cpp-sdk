#include <viam/sdk/config/resource.hpp>

#include <numeric>
#include <string>

#include <boost/algorithm/string.hpp>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/referenceframe/frame.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

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

const ProtoStruct& ResourceConfig::attributes() const {
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
        throw Exception("component namespace and/or type do not match component api field");
    }
}

const std::vector<std::string>& ResourceConfig::depends_on() const {
    return depends_on_;
};

const std::vector<ResourceLevelServiceConfig>& ResourceConfig::service_config() const {
    return service_config_;
};

ResourceConfig::ResourceConfig(std::string type,
                               std::string name,
                               std::string namespace_,
                               ProtoStruct attributes,
                               std::string api,
                               Model model,
                               LinkConfig frame)
    : api_({kRDK, type, ""}),
      frame_(std::move(frame)),
      model_(std::move(model)),
      name_(std::move(name)),
      namespace__(std::move(namespace_)),
      type_(std::move(type)),
      attributes_(std::move(attributes)) {
    if (api.find(':') != std::string::npos) {
        api_ = API::from_string(std::move(api));
    }
    fix_api();
}

ResourceConfig::ResourceConfig(std::string type)
    : api_({kRDK, type, ""}), type_(std::move(type)) {};

}  // namespace sdk
}  // namespace viam
