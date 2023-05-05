#include "viam/sdk/common/proto_type.hpp"
#include <vector>
#include <viam/sdk/tests/mocks/mock_robot.hpp>

#include <common/v1/common.pb.h>
#include <robot/v1/robot.pb.h>

namespace viam {
namespace sdktests {
namespace robot {
using namespace viam::sdk;

common::v1::Pose default_pose(int offset = 0) {
    common::v1::Pose pose;
    pose.set_x(1 + offset);
    pose.set_y(2 + offset);
    pose.set_z(3 + offset);
    pose.set_o_x(2 + offset);
    pose.set_o_y(3 + offset);
    pose.set_o_z(4 + offset);
    pose.set_theta(20 + offset);
    return pose;
}

std::vector<viam::robot::v1::Operation> mock_operations_response() {
    viam::robot::v1::Operation op;
    *op.mutable_id() = "abc";
    *op.mutable_method() = "123";
    *op.mutable_session_id() = "jkl";
    viam::robot::v1::Operation op1;
    *op.mutable_id() = "def";
    *op.mutable_method() = "456";
    *op.mutable_session_id() = "xyz";
    std::vector<viam::robot::v1::Operation> resp;
    resp.push_back(op);
    resp.push_back(op1);
    return resp;
}

std::vector<viam::robot::v1::Discovery> mock_discovery_response() {
    viam::robot::v1::DiscoveryQuery query;
    *query.mutable_subtype() = "camera";
    *query.mutable_model() = "webcam";

    google::protobuf::Struct results;
    google::protobuf::Value str;
    *str.mutable_string_value() = "bar";
    google::protobuf::MapPair<std::string, google::protobuf::Value> pair("foo", str);

    google::protobuf::Map<std::string, google::protobuf::Value>* map = results.mutable_fields();
    map->insert(pair);

    viam::robot::v1::Discovery discovery;
    *discovery.mutable_query() = query;
    *discovery.mutable_results() = results;
    return std::vector<viam::robot::v1::Discovery>{discovery};
}

std::vector<viam::robot::v1::Status> mock_status_response() {
    auto rns = mock_resource_names_response();

    viam::robot::v1::Status camera_status;
    *camera_status.mutable_name() = rns[1];
    *camera_status.mutable_status() = google::protobuf::Struct();

    viam::robot::v1::Status generic_status;
    *generic_status.mutable_name() = rns[0];
    *generic_status.mutable_status() = google::protobuf::Struct();

    viam::robot::v1::Status motor_status;
    *motor_status.mutable_name() = rns[2];
    *motor_status.mutable_status() = google::protobuf::Struct();

    std::vector<viam::robot::v1::Status> resp;
    resp.push_back(camera_status);
    resp.push_back(generic_status);
    resp.push_back(motor_status);

    return resp;
}

common::v1::PoseInFrame mock_transform_response() {
    common::v1::PoseInFrame response;
    *response.mutable_reference_frame() = "arm";
    *response.mutable_pose() = default_pose();
    return response;
}

std::vector<ResourceName> mock_resource_names_response() {
    std::vector<ResourceName> vec;
    ResourceName camera;
    *camera.mutable_namespace_() = RDK;
    *camera.mutable_type() = COMPONENT;
    *camera.mutable_name() = "mock_camera";
    *camera.mutable_subtype() = "camera";

    ResourceName generic;
    *generic.mutable_namespace_() = RDK;
    *generic.mutable_type() = COMPONENT;
    *generic.mutable_name() = "mock_generic";
    *generic.mutable_subtype() = "generic";

    ResourceName motor;
    *motor.mutable_namespace_() = RDK;
    *motor.mutable_type() = COMPONENT;
    *motor.mutable_name() = "mock_motor";
    *motor.mutable_subtype() = "motor";

    vec.push_back(camera);
    vec.push_back(generic);
    vec.push_back(motor);
    return vec;
}

std::vector<FrameSystemConfig> mock_config_response() {
    FrameSystemConfig config;
    common::v1::Transform t;
    *t.mutable_reference_frame() = "some-reference-frame";
    viam::common::v1::Pose pose = default_pose();
    common::v1::PoseInFrame pif;
    *pif.mutable_reference_frame() = "reference0";
    *pif.mutable_pose() = pose;
    *t.mutable_pose_in_observer_frame() = pif;
    *config.mutable_frame() = t;
    google::protobuf::Struct s;
    google::protobuf::Value v;
    v.set_number_value(1);
    google::protobuf::MapPair<std::string, google::protobuf::Value> pair("fake-key", v);
    s.mutable_fields()->insert(pair);
    *config.mutable_kinematics() = s;

    FrameSystemConfig config1;
    common::v1::Transform t1;
    *t1.mutable_reference_frame() = "another-reference-frame";
    viam::common::v1::Pose pose1 = default_pose(1);
    common::v1::PoseInFrame pif1;
    *pif.mutable_reference_frame() = "reference1";
    *pif.mutable_pose() = pose1;
    *t1.mutable_pose_in_observer_frame() = pif1;
    *config.mutable_frame() = t1;
    google::protobuf::Struct s1;
    google::protobuf::Value v1;
    v1.set_number_value(2);
    google::protobuf::MapPair<std::string, google::protobuf::Value> pair1("new-fake-key", v1);
    s1.mutable_fields()->insert(pair);
    *config1.mutable_kinematics() = s1;

    std::vector<FrameSystemConfig> response;
    response.push_back(config);
    response.push_back(config1);
    return response;
}

::grpc::Status MockRobotService::FrameSystemConfig(
    ::grpc::ServerContext* context,
    const ::viam::robot::v1::FrameSystemConfigRequest* request,
    ::viam::robot::v1::FrameSystemConfigResponse* response) {
    auto* configs = response->mutable_frame_system_configs();
    for (const auto& c : mock_config_response()) {
        *configs->Add() = c;
    }
    return ::grpc::Status();
}

::grpc::Status MockRobotService::DiscoverComponents(
    ::grpc::ServerContext* context,
    const ::viam::robot::v1::DiscoverComponentsRequest* request,
    ::viam::robot::v1::DiscoverComponentsResponse* response) {
    auto* discovery = response->mutable_discovery();
    for (auto& d : mock_discovery_response()) {
        *discovery->Add() = d;
    }
    return ::grpc::Status();
}

::grpc::Status MockRobotService::TransformPose(
    ::grpc::ServerContext* context,
    const ::viam::robot::v1::TransformPoseRequest* request,
    ::viam::robot::v1::TransformPoseResponse* response) {
    *response->mutable_pose() = mock_transform_response();
    return ::grpc::Status();
}

::grpc::Status MockRobotService::GetOperations(
    ::grpc::ServerContext* context,
    const ::viam::robot::v1::GetOperationsRequest* request,
    ::viam::robot::v1::GetOperationsResponse* response) {
    auto* ops = response->mutable_operations();
    for (auto& op : mock_operations_response()) {
        *ops->Add() = op;
    }
    return ::grpc::Status();
}

}  // namespace robot
}  // namespace sdktests
}  // namespace viam
