#pragma once

#include <string>
#include <unordered_map>

#include <app/v1/robot.pb.h>
#include <robot/v1/robot.pb.h>

#include <common/proto_type.hpp>
#include <referenceframe/frame.hpp>
#include <resource/resource.hpp>

namespace viam {
namespace cppsdk {

typedef std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> AttributeMap;

class ResourceLevelServiceConfig {
   public:
    std::string type;
    AttributeMap attributes;
    ProtoType converted_attributes;
};

class Resource {
   public:
    std::string name;
    std::string namespace_;
    std::string type;
    Subtype api;
    Model model;
    LinkConfig frame;
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

}  // namespace cppsdk
}  // namespace viam
