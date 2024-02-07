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

std::vector<RobotClient::operation> mock_operations_response() {
    RobotClient::operation op;
    op.id = "abc";
    op.method = "123";
    op.session_id = "jkl";
    RobotClient::operation op1;
    op1.id = "def";
    op1.method = "456";
    op1.session_id = "xyz";
    std::vector<RobotClient::operation> resp;
    resp.push_back(op);
    resp.push_back(op1);
    return resp;
}

std::vector<RobotClient::discovery> mock_discovery_response() {
    RobotClient::discovery_query query;
    query.subtype = "camera";
    query.model = "webcam";

    AttributeMap results =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();
    google::protobuf::Value v;
    *v.mutable_string_value() = "bar";
    google::protobuf::MapPair<std::string, std::shared_ptr<ProtoType>> pair(
        std::move("foo"), std::make_shared<ProtoType>(v));
    results->emplace(pair);

    RobotClient::discovery discovery;
    discovery.query = query;
    discovery.results = results;
    return std::vector<RobotClient::discovery>{discovery};
}

std::vector<viam::robot::v1::Status> mock_status_response() {
    auto rns = mock_resource_names_response();

    viam::robot::v1::Status camera_status;
    *camera_status.mutable_name() = rns[1].to_proto();
    *camera_status.mutable_status() = google::protobuf::Struct();

    viam::robot::v1::Status generic_status;
    *generic_status.mutable_name() = rns[0].to_proto();
    *generic_status.mutable_status() = google::protobuf::Struct();

    viam::robot::v1::Status motor_status;
    *motor_status.mutable_name() = rns[2].to_proto();
    *motor_status.mutable_status() = google::protobuf::Struct();

    std::vector<viam::robot::v1::Status> resp;
    resp.push_back(camera_status);
    resp.push_back(generic_status);
    resp.push_back(motor_status);

    return resp;
}

pose_in_frame mock_transform_response() {
    return pose_in_frame("arm", pose::from_proto(default_pose()));
}

std::vector<Name> mock_resource_names_response() {
    std::vector<Name> vec;
    Name camera = Name({kRDK, kComponent, "camera"}, "", "mock_camera");
    Name generic = Name({kRDK, kComponent, "generic"}, "", "mock_generic");
    Name motor = Name({kRDK, kComponent, "motor"}, "", "mock_motor");

    vec.push_back(camera);
    vec.push_back(generic);
    vec.push_back(motor);
    return vec;
}

std::vector<viam::robot::v1::FrameSystemConfig> mock_config_response() {
    viam::robot::v1::FrameSystemConfig config;
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

    viam::robot::v1::FrameSystemConfig config1;
    common::v1::Transform t1;
    *t1.mutable_reference_frame() = "another-reference-frame";
    viam::common::v1::Pose pose1 = default_pose(1);
    common::v1::PoseInFrame pif1;
    *pif1.mutable_reference_frame() = "reference1";
    *pif1.mutable_pose() = pose1;
    *t1.mutable_pose_in_observer_frame() = pif1;
    *config1.mutable_frame() = t1;
    google::protobuf::Struct s1;
    google::protobuf::Value v1;
    v1.set_number_value(2);
    google::protobuf::MapPair<std::string, google::protobuf::Value> pair1("new-fake-key", v1);
    s1.mutable_fields()->insert(pair);
    *config1.mutable_kinematics() = s1;

    std::vector<viam::robot::v1::FrameSystemConfig> response;
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
        *discovery->Add() = d.to_proto();
    }
    return ::grpc::Status();
}

::grpc::Status MockRobotService::TransformPose(
    ::grpc::ServerContext* context,
    const ::viam::robot::v1::TransformPoseRequest* request,
    ::viam::robot::v1::TransformPoseResponse* response) {
    *response->mutable_pose() = mock_transform_response().to_proto();
    return ::grpc::Status();
}

::grpc::Status MockRobotService::GetOperations(
    ::grpc::ServerContext* context,
    const ::viam::robot::v1::GetOperationsRequest* request,
    ::viam::robot::v1::GetOperationsResponse* response) {
    auto* ops = response->mutable_operations();
    for (auto& op : mock_operations_response()) {
        *ops->Add() = op.to_proto();
    }
    return ::grpc::Status();
}

}  // namespace robot
}  // namespace sdktests
}  // namespace viam
