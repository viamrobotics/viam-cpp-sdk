#include <boost/optional.hpp>
#include <string>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>

#include "viam/app/v1/app.pb.h"

namespace viam {
namespace sdk {

struct RobotPart {
    std::string name;
    std::string id;
    std::vector<std::string> attributes;
    boost::optional<ProtoStruct> robot_config;
    // The robot's configuration as a JSON string.
    boost::optional<std::string> robot_config_json;
};

namespace proto_convert_details {

template <>
struct to_proto_impl<RobotPart> {
    std::unique_ptr<viam::app::v1::RobotPart> operator()(const RobotPart& sdk_part) {
        auto proto_part = std::make_unique<viam::app::v1::RobotPart>();
        proto_part->set_name(sdk_part.name);
        proto_part->set_id(sdk_part.id);
        for (const auto& attr : sdk_part.attributes) {
            proto_part->add_attributes(attr);
        }
        if (sdk_part.robot_config) {
            proto_part->mutable_robot_config()->CopyFrom(*sdk_part.robot_config);
        }
        if (sdk_part.robot_config_json) {
            proto_part->set_robot_config_json(*sdk_part.robot_config_json);
        }
        return proto_part;
    }
};

template <>
struct from_proto_impl<viam::app::v1::RobotPart> {
    RobotPart operator()(const viam::app::v1::RobotPart& proto_part) {
        RobotPart sdk_part;
        sdk_part.name = proto_part.name();
        sdk_part.id = proto_part.id();
        for (const auto& attr : proto_part.attributes()) {
            sdk_part.attributes.push_back(attr);
        }
        if (proto_part.has_robot_config()) {
            sdk_part.robot_config = proto_part.robot_config();
        }
        if (proto_part.has_robot_config_json()) {
            sdk_part.robot_config_json = proto_part.robot_config_json();
        }
        return sdk_part;
    }
};

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam
