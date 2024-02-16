#pragma once

#include <string>
#include <unordered_map>

#include <viam/api/app/v1/robot.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/referenceframe/frame.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdk {

class ResourceLevelServiceConfig {
   public:
    std::string type;
    AttributeMap attributes;
    ProtoType converted_attributes;
};

class ResourceConfig {
   public:
    static ResourceConfig from_proto(const viam::app::v1::ComponentConfig& proto_cfg);
    viam::app::v1::ComponentConfig to_proto() const;
    ResourceConfig(std::string type);
    Name resource_name();
    const API& api() const;
    const LinkConfig& frame() const;
    const Model& model() const;
    const std::string& name() const;
    const std::string& namespace_() const;
    const std::string& type() const;
    const AttributeMap& attributes() const;

   private:
    API api_;
    LinkConfig frame_;
    Model model_;
    std::string name_;
    std::string namespace__;
    std::string type_;
    std::vector<std::string> depends_on_;
    std::vector<ResourceLevelServiceConfig> service_config_;
    AttributeMap attributes_;
    ProtoType converted_attributes_;
    std::vector<std::string> implicit_depends_on_;
    void fix_api();
};

}  // namespace sdk
}  // namespace viam
