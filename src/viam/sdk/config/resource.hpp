#pragma once

#include <string>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/referenceframe/frame.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdk {

class ResourceLevelServiceConfig {
   public:
    std::string type;
    ProtoStruct attributes;
    ProtoValue converted_attributes;
};

class ResourceConfig {
   public:
    ResourceConfig(std::string type);
    ResourceConfig(std::string type,
                   std::string name,
                   std::string namespace_,
                   ProtoStruct attributes,
                   std::string api,
                   Model model,
                   LinkConfig frame);
    Name resource_name();
    const API& api() const;
    const LinkConfig& frame() const;
    const Model& model() const;
    const std::string& name() const;
    const std::string& namespace_() const;
    const std::string& type() const;
    const ProtoStruct& attributes() const;
    const std::vector<std::string>& depends_on() const;
    const std::vector<ResourceLevelServiceConfig>& service_config() const;

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

}  // namespace sdk
}  // namespace viam
