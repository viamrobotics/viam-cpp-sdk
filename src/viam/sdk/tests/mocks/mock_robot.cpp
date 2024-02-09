#include "viam/sdk/common/proto_type.hpp"
#include <vector>
#include <viam/sdk/tests/mocks/mock_robot.hpp>

#include <common/v1/common.pb.h>
#include <robot/v1/robot.pb.h>

namespace viam {
namespace sdktests {
namespace robot {
using namespace viam::sdk;

pose default_pose(int offset = 0) {
    pose pose;
    pose.coordinates.x = 1 + offset;
    pose.coordinates.y = 2 + offset;
    pose.coordinates.z = 3 + offset;
    pose.orientation.o_x = 2 + offset;
    pose.orientation.o_y = 3 + offset;
    pose.orientation.o_z = 4 + offset;
    pose.theta = 20 + offset;
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
    results->emplace("foo", std::make_shared<ProtoType>("bar"));

    RobotClient::discovery discovery;
    discovery.query = query;
    discovery.results = results;
    return std::vector<RobotClient::discovery>{discovery};
}

std::vector<RobotClient::status> mock_status_response() {
    auto rns = mock_resource_names_response();

    RobotClient::status camera_status;
    camera_status.name = rns[0];
    camera_status.status_map =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();

    RobotClient::status motor_status;
    motor_status.name = rns[1];
    motor_status.status_map =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();

    RobotClient::status generic_status;
    generic_status.name = rns[2];
    generic_status.status_map =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();

    std::vector<RobotClient::status> resp;
    resp.push_back(camera_status);
    resp.push_back(motor_status);
    resp.push_back(generic_status);

    return resp;
}

pose_in_frame mock_transform_response() {
    return {"arm", default_pose()};
}

std::vector<Name> mock_resource_names_response() {
    Name camera = Name({kRDK, kComponent, "camera"}, "", "mock_camera");
    Name motor = Name({kRDK, kComponent, "motor"}, "", "mock_motor");
    Name generic = Name({kRDK, kComponent, "generic"}, "", "mock_generic");

    return {std::move(camera), std::move(motor), std::move(generic)};
}

std::vector<RobotClient::frame_system_config> mock_config_response() {
    RobotClient::frame_system_config config;
    WorldState::transform t;
    t.reference_frame = "some-reference-frame";
    pose_in_frame pif("reference0", default_pose());
    t.pose_in_observer_frame = pif;
    config.frame = t;
    AttributeMap kinematics =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();
    kinematics->emplace(std::move("fake-key"), std::make_shared<ProtoType>(1));
    config.kinematics = kinematics;

    RobotClient::frame_system_config config1;
    WorldState::transform t1;
    t1.reference_frame = "another-reference-frame";
    pose_in_frame pif1("reference1", default_pose(1));
    t1.pose_in_observer_frame = pif1;
    config1.frame = t1;
    AttributeMap kinematics1 =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();
    kinematics1->emplace(std::move("new-fake-key"), std::make_shared<ProtoType>(2));
    config1.kinematics = kinematics1;

    std::vector<RobotClient::frame_system_config> response;
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
        *configs->Add() = c.to_proto();
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
