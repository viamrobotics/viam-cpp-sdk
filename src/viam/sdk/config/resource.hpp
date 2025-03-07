#pragma once

#include <string>
#include <unordered_map>

#include <viam/sdk/common/proto_convert.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/log/logger.hpp>
#include <viam/sdk/referenceframe/frame.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace app {
namespace v1 {

class ComponentConfig;
class ResourceLevelServiceConfig;

}  // namespace v1
}  // namespace app

namespace service {
namespace discovery {
namespace v1 {

class DiscoverResourcesResponse;
}
}  // namespace discovery
}  // namespace service
}  // namespace viam

namespace viam {
namespace sdk {

struct ResourceLevelServiceConfig {
    std::string type;
    ProtoStruct attributes;
    ProtoValue converted_attributes;
};

class ResourceConfig {
   public:
    ResourceConfig(std::string type,
                   std::string name,
                   std::string namespace_,
                   ProtoStruct attributes,
                   std::string api,
                   Model model,
                   LinkConfig frame,
                   sdk::log_level lvl = sdk::log_level::info);

    ResourceConfig(std::string type);

    Name resource_name();

    const API& api() const;

    const LinkConfig& frame() const;

    const Model& model() const;

    const std::string& name() const;
    const std::string& namespace_() const;
    const std::string& type() const;

    log_level log_level() const;

    const std::vector<std::string>& depends_on() const;

    const std::vector<ResourceLevelServiceConfig>& service_config() const;

    const ProtoStruct& attributes() const;

   private:
    void fix_api();

    API api_;

    LinkConfig frame_;

    Model model_;

    std::string name_;
    std::string namespace__;
    std::string type_;

    std::vector<std::string> depends_on_;

    std::vector<ResourceLevelServiceConfig> service_config_;

    ProtoStruct attributes_;
    ProtoValue converted_attributes_;

    std::vector<std::string> implicit_depends_on_;

    sdk::log_level log_level_;
};

namespace proto_convert_details {

template <>
struct to_proto_impl<ResourceLevelServiceConfig> {
    void operator()(const ResourceLevelServiceConfig&, app::v1::ResourceLevelServiceConfig*) const;
};

template <>
struct to_proto_impl<ResourceConfig> {
    void operator()(const ResourceConfig&, app::v1::ComponentConfig*) const;
};

template <>
struct from_proto_impl<app::v1::ComponentConfig> {
    ResourceConfig operator()(const app::v1::ComponentConfig*) const;
};

template <>
struct from_proto_impl<service::discovery::v1::DiscoverResourcesResponse> {
    std::vector<ResourceConfig> operator()(
        const service::discovery::v1::DiscoverResourcesResponse*) const;
};

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam
