#include <viam/sdk/tests/mocks/mock_robot.hpp>

#include <vector>

#include <grpcpp/support/status.h>

#include <common/v1/common.pb.h>
#include <robot/v1/robot.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace robot {
using namespace viam::sdk;
using common::v1::Pose;
using common::v1::PoseInFrame;
using viam::robot::v1::Discovery;
using viam::robot::v1::DiscoveryQuery;
using viam::robot::v1::FrameSystemConfig;
using viam::robot::v1::Operation;
using viam::robot::v1::Status;

pose default_pose(int offset) {
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

Pose default_proto_pose(int offset = 0) {
    return default_pose(offset).to_proto();
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

std::vector<Operation> mock_proto_operations_response() {
    viam::robot::v1::Operation op;
    *op.mutable_id() = "abc";
    *op.mutable_method() = "123";
    *op.mutable_session_id() = "jkl";
    viam::robot::v1::Operation op1;
    *op1.mutable_id() = "def";
    *op1.mutable_method() = "456";
    *op1.mutable_session_id() = "xyz";
    std::vector<viam::robot::v1::Operation> resp;
    resp.push_back(op);
    resp.push_back(op1);
    return resp;
}

std::vector<RobotClient::discovery> mock_discovery_response() {
    RobotClient::discovery_query query;
    query.subtype = "camera";
    query.model = "webcam";
    auto s = std::make_shared<ProtoType>("bar");
    auto map_pt = std::make_shared<ProtoType>(fake_map());
    std::vector<std::shared_ptr<ProtoType>> inner{map_pt};
    std::vector<std::shared_ptr<ProtoType>> l_{s,
                                               std::make_shared<ProtoType>(false),
                                               std::make_shared<ProtoType>(3),
                                               std::make_shared<ProtoType>(4.2),
                                               map_pt,
                                               std::make_shared<ProtoType>(inner)};
    auto l = std::make_shared<ProtoType>(l_);

    AttributeMap results =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();
    results->emplace("foo", l);

    RobotClient::discovery discovery;
    discovery.query = std::move(query);
    discovery.results = std::move(results);
    return std::vector<RobotClient::discovery>{std::move(discovery)};
}

std::vector<Discovery> mock_proto_discovery_response() {
    std::vector<viam::robot::v1::Discovery> v;
    for (const auto& d : mock_discovery_response()) {
        DiscoveryQuery query;
        *query.mutable_subtype() = d.query.subtype;
        *query.mutable_model() = d.query.model;

        viam::robot::v1::Discovery discovery;
        *discovery.mutable_query() = query;
        *discovery.mutable_results() = map_to_struct(d.results);

        v.push_back(std::move(discovery));
    }

    return v;
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

    return {std::move(camera_status), std::move(motor_status), std::move(generic_status)};
}

std::vector<Status> mock_proto_status_response() {
    auto rns = mock_proto_resource_names_response();

    Status camera_status;
    *camera_status.mutable_name() = rns[0];
    *camera_status.mutable_status() = google::protobuf::Struct();

    Status motor_status;
    *motor_status.mutable_name() = rns[1];
    *motor_status.mutable_status() = google::protobuf::Struct();

    Status generic_status;
    *generic_status.mutable_name() = rns[2];
    *generic_status.mutable_status() = google::protobuf::Struct();

    std::vector<Status> resp;
    resp.push_back(camera_status);
    resp.push_back(motor_status);
    resp.push_back(generic_status);

    return resp;
}

pose_in_frame mock_transform_response() {
    return {"arm", default_pose()};
}

PoseInFrame mock_proto_transform_response() {
    PoseInFrame response;
    *response.mutable_reference_frame() = "arm";
    *response.mutable_pose() = default_proto_pose();
    return response;
}

std::vector<Name> mock_resource_names_response() {
    Name camera = Name({kRDK, kComponent, "camera"}, "", "mock_camera");
    Name motor = Name({kRDK, kComponent, "motor"}, "", "mock_motor");
    Name generic = Name({kRDK, kComponent, "generic"}, "", "mock_generic");

    return {std::move(camera), std::move(motor), std::move(generic)};
}

std::vector<ResourceName> mock_proto_resource_names_response() {
    std::vector<ResourceName> vec;
    ResourceName camera;
    *camera.mutable_namespace_() = kRDK;
    *camera.mutable_type() = kComponent;
    *camera.mutable_name() = "mock_camera";
    *camera.mutable_subtype() = "camera";

    ResourceName generic;
    *generic.mutable_namespace_() = kRDK;
    *generic.mutable_type() = kComponent;
    *generic.mutable_name() = "mock_generic";
    *generic.mutable_subtype() = "generic";

    ResourceName motor;
    *motor.mutable_namespace_() = kRDK;
    *motor.mutable_type() = kComponent;
    *motor.mutable_name() = "mock_motor";
    *motor.mutable_subtype() = "motor";

    vec.push_back(camera);
    vec.push_back(motor);
    vec.push_back(generic);
    return vec;
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

std::vector<FrameSystemConfig> mock_proto_config_response() {
    FrameSystemConfig config;
    common::v1::Transform t;
    *t.mutable_reference_frame() = "some-reference-frame";
    Pose pose = default_proto_pose();
    PoseInFrame pif;
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
    Pose pose1 = default_proto_pose(1);
    PoseInFrame pif1;
    *pif1.mutable_reference_frame() = "reference1";
    *pif1.mutable_pose() = pose1;
    *t1.mutable_pose_in_observer_frame() = pif1;
    *config1.mutable_frame() = t1;
    google::protobuf::Struct s1;
    google::protobuf::Value v1;
    v1.set_number_value(2);
    google::protobuf::MapPair<std::string, google::protobuf::Value> pair1("new-fake-key", v1);
    s1.mutable_fields()->insert(pair1);
    *config1.mutable_kinematics() = s1;

    std::vector<FrameSystemConfig> response;
    response.push_back(config);
    response.push_back(config1);
    return response;
}

::grpc::Status MockRobotService::FrameSystemConfig(
    ::grpc::ServerContext* context,
    const ::viam::robot::v1::FrameSystemConfigRequest*,
    ::viam::robot::v1::FrameSystemConfigResponse* response) {
    auto client_md = context->client_metadata();
    if (auto client_info = client_md.find("viam_client"); client_info == client_md.end()) {
        return ::grpc::Status(::grpc::StatusCode::FAILED_PRECONDITION,
                              "viam_client info not properly set in metadata");
    }
    auto* configs = response->mutable_frame_system_configs();
    for (const auto& c : mock_proto_config_response()) {
        *configs->Add() = c;
    }
    return ::grpc::Status();
}

::grpc::Status MockRobotService::DiscoverComponents(
    ::grpc::ServerContext* context,
    const ::viam::robot::v1::DiscoverComponentsRequest*,
    ::viam::robot::v1::DiscoverComponentsResponse* response) {
    auto client_md = context->client_metadata();
    if (auto client_info = client_md.find("viam_client"); client_info == client_md.end()) {
        return ::grpc::Status(::grpc::StatusCode::FAILED_PRECONDITION,
                              "viam_client info not properly set in metadata");
    }
    auto* discovery = response->mutable_discovery();
    for (auto& d : mock_proto_discovery_response()) {
        *discovery->Add() = d;
    }
    return ::grpc::Status();
}

::grpc::Status MockRobotService::TransformPose(::grpc::ServerContext* context,
                                               const ::viam::robot::v1::TransformPoseRequest*,
                                               ::viam::robot::v1::TransformPoseResponse* response) {
    auto client_md = context->client_metadata();
    if (auto client_info = client_md.find("viam_client"); client_info == client_md.end()) {
        return ::grpc::Status(::grpc::StatusCode::FAILED_PRECONDITION,
                              "viam_client info not properly set in metadata");
    }
    *response->mutable_pose() = mock_proto_transform_response();
    return ::grpc::Status();
}

::grpc::Status MockRobotService::GetOperations(::grpc::ServerContext* context,
                                               const ::viam::robot::v1::GetOperationsRequest*,
                                               ::viam::robot::v1::GetOperationsResponse* response) {
    auto client_md = context->client_metadata();
    if (auto client_info = client_md.find("viam_client"); client_info == client_md.end()) {
        return ::grpc::Status(::grpc::StatusCode::FAILED_PRECONDITION,
                              "viam_client info not properly set in metadata");
    }
    auto* ops = response->mutable_operations();
    for (auto& op : mock_proto_operations_response()) {
        *ops->Add() = op;
    }
    return ::grpc::Status();
}

}  // namespace robot
}  // namespace sdktests
}  // namespace viam
