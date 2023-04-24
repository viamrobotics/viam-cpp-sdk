#pragma once

#include <string>
#include <unordered_map>

#include <viam/api/app/v1/robot.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/referenceframe/frame.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

typedef std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> AttributeMap;

class ResourceLevelServiceConfig {
   public:
    std::string type;
    AttributeMap attributes;
    ProtoType converted_attributes;
};

class Resource {
   public:
    Subtype api;
    LinkConfig frame;
    Model model;
    std::string name;
    std::string namespace_;
    std::string type;
    std::vector<std::string> depends_on;
    std::vector<ResourceLevelServiceConfig> service_config;
    AttributeMap attributes;
    ProtoType converted_attributes;
    std::vector<std::string> implicit_depends_on;

    static Resource from_proto(viam::app::v1::ComponentConfig proto_cfg);
    viam::app::v1::ComponentConfig to_proto();
    Resource(std::string type);
    Name resource_name();

   private:
    void fix_api();
};

}  // namespace sdk
}  // namespace viam
