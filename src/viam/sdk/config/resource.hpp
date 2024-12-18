#pragma once

#include <string>
#include <unordered_map>

#include <viam/sdk/common/proto_convert.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/referenceframe/frame.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace app {
namespace v1 {

class ComponentConfig;
class ResourceLevelServiceConfig;

}  // namespace v1
}  // namespace app
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
                   LinkConfig frame);

    ResourceConfig(std::string type);

    Name resource_name();
    const API& api() const;
    const LinkConfig& frame() const;
    const Model& model() const;
    const std::string& name() const;
    const std::string& namespace_() const;
    const std::string& type() const;
    const std::vector<std::string>& depends_on() const;
    const std::vector<ResourceLevelServiceConfig>& service_config() const;
    const ProtoStruct& attributes() const;

   private:
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
    void fix_api();
};

namespace proto_convert_details {

template <>
struct to_proto<ResourceLevelServiceConfig> {
    void operator()(const ResourceLevelServiceConfig&, app::v1::ResourceLevelServiceConfig*) const;
};

template <>
struct to_proto<ResourceConfig> {
    void operator()(const ResourceConfig&, app::v1::ComponentConfig*) const;
};

template <>
struct from_proto<app::v1::ComponentConfig> {
    ResourceConfig operator()(const app::v1::ComponentConfig*) const;
};

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam
