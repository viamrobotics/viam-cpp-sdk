#include <memory>
#include <utility>

#include "../gen/mock/robot_mock.grpc.pb.h"
#include "../src/robot/service.h"
#include "common/v1/common.pb.h"
#include "component/arm/v1/arm.grpc.pb.h"
#include "component/arm/v1/arm.pb.h"
#include "google/protobuf/struct.pb.h"
#include "grpcpp/channel.h"
#include "grpcpp/client_context.h"
#include "grpcpp/create_channel.h"
#include "grpcpp/impl/channel_interface.h"
#include "grpcpp/security/credentials.h"
#include "grpcpp/server.h"
#include "grpcpp/server_builder.h"
#include "grpcpp/server_context.h"
#include "grpcpp/support/server_callback.h"
#include "grpcpp/support/stub_options.h"
#include "robot/v1/robot.grpc.pb.h"
#include "robot/v1/robot.pb.h"

using google::protobuf::RepeatedPtrField;
using viam::common::v1::PoseInFrame;
using viam::common::v1::ResourceName;
class TestService : public RobotService_ {
       public:
	::grpc::Status FrameSystemConfig(
	    ::grpc::ServerContext* context,
	    const ::viam::robot::v1::FrameSystemConfigRequest* request,
	    ::viam::robot::v1::FrameSystemConfigResponse* response) override;

	::grpc::Status TransformPose(
	    ::grpc::ServerContext* context,
	    const ::viam::robot::v1::TransformPoseRequest* request,
	    ::viam::robot::v1::TransformPoseResponse* response) override;

	::grpc::Status DiscoverComponents(
	    ::grpc::ServerContext* context,
	    const ::viam::robot::v1::DiscoverComponentsRequest* request,
	    ::viam::robot::v1::DiscoverComponentsResponse* response) override;

	::grpc::Status GetOperations(
	    ::grpc::ServerContext* context,
	    const ::viam::robot::v1::GetOperationsRequest* request,
	    ::viam::robot::v1::GetOperationsResponse* response) override;

	TestService();
};

class MockStub : public RobotService::StubInterface {
	MockStub(TestService service){};
};

std::vector<ResourceName> names_for_testing() {
	std::vector<ResourceName> vec;
	ResourceName arm1;
	*arm1.mutable_namespace_() = "rdk";
	*arm1.mutable_type() = "component";
	*arm1.mutable_name() = "arm";
	*arm1.mutable_subtype() = "arm1";

	ResourceName camera1;
	*camera1.mutable_namespace_() = "rdk";
	*camera1.mutable_type() = "component";
	*camera1.mutable_name() = "camera";
	*camera1.mutable_subtype() = "camera1";

	ResourceName motor1;
	*motor1.mutable_namespace_() = "rdk";
	*motor1.mutable_type() = "component";
	*motor1.mutable_name() = "motor";
	*motor1.mutable_subtype() = "motor1";

	vec.push_back(arm1);
	vec.push_back(camera1);
	vec.push_back(motor1);
	return vec;
}

viam::common::v1::Pose default_pose() {
	viam::common::v1::Pose pose;
	pose.set_x(1);
	pose.set_y(2);
	pose.set_z(3);
	pose.set_o_x(2);
	pose.set_o_y(3);
	pose.set_o_z(4);
	pose.set_theta(20);
	return pose;
}

RepeatedPtrField<FrameSystemConfig> config_response() {
	FrameSystemConfig config;
	*config.mutable_name() = "config0";
	*config.mutable_model_json() = "some fake json";
	viam::common::v1::Pose pose = default_pose();
	PoseInFrame pif;
	*pif.mutable_reference_frame() = "reference0";
	*pif.mutable_pose() = pose;
	*config.mutable_pose_in_parent_frame() = pif;

	FrameSystemConfig config1;
	*config1.mutable_name() = "config1";
	*config1.mutable_model_json() = "some fake json part 2";
	viam::common::v1::Pose pose1;
	pose.set_x(2);
	pose.set_y(3);
	pose.set_z(4);
	pose.set_o_x(3);
	pose.set_o_y(4);
	pose.set_o_z(5);
	pose.set_theta(21);
	PoseInFrame pif1;
	*pif1.mutable_reference_frame() = "reference1";
	*pif.mutable_pose() = pose1;
	*config1.mutable_pose_in_parent_frame() = pif1;

	RepeatedPtrField<FrameSystemConfig> response;
	*response.Add() = config;
	*response.Add() = config1;
	return response;
}

PoseInFrame transform_response() {
	PoseInFrame response;
	*response.mutable_reference_frame() = "arm";
	*response.mutable_pose() = default_pose();
	return response;
}

RepeatedPtrField<viam::robot::v1::Discovery> discovery_response() {
	viam::robot::v1::DiscoveryQuery query;
	*query.mutable_subtype() = "camera";
	*query.mutable_model() = "webcam";

	google::protobuf::Struct results;
	google::protobuf::Value str;
	*str.mutable_string_value() = "bar";
	std::pair<std::string, google::protobuf::Value> str_pair("foo", str);

	google::protobuf::Value i;
	i.set_number_value(1);
	std::pair<std::string, google::protobuf::Value> int_pair("one", i);

	google::protobuf::Map<std::string, google::protobuf::Value>* map =
	    results.mutable_fields();
	map->emplace(str_pair);
	map->emplace(int_pair);

	viam::robot::v1::Discovery discovery;
	*discovery.mutable_query() = query;
	*discovery.mutable_results() = results;
	RepeatedPtrField<viam::robot::v1::Discovery> resp;
	*resp.Add() = discovery;
	return resp;
}

RepeatedPtrField<viam::robot::v1::Operation> operations_response() {
	viam::robot::v1::Operation op;
	*op.mutable_id() = "abc";
	RepeatedPtrField<viam::robot::v1::Operation> resp;
	*resp.Add() = op;
	return resp;
}

viam::component::arm::v1::Status default_status() {
	viam::common::v1::Pose pose;
	pose.set_x(1);
	pose.set_y(2);
	pose.set_z(3);
	pose.set_o_x(4);
	pose.set_o_y(5);
	pose.set_o_z(6);
	pose.set_theta(20);

	viam::component::arm::v1::JointPositions jp;
	for (auto i = 0; i < 6; ++i) {
		jp.add_values(0);
	}

	viam::component::arm::v1::Status status;
	*status.mutable_end_position() = pose;
	*status.mutable_joint_positions() = jp;
	status.set_is_moving(false);

	return status;
}

::grpc::Status TestService::FrameSystemConfig(
    ::grpc::ServerContext* context,
    const ::viam::robot::v1::FrameSystemConfigRequest* request,
    ::viam::robot::v1::FrameSystemConfigResponse* response) {
	*response->mutable_frame_system_configs() = config_response();
	return ::grpc::Status();
}

::grpc::Status TestService::TransformPose(
    ::grpc::ServerContext* context,
    const ::viam::robot::v1::TransformPoseRequest* request,
    ::viam::robot::v1::TransformPoseResponse* response) {
	*response->mutable_pose() = transform_response();
	return ::grpc::Status();
}

::grpc::Status TestService::DiscoverComponents(
    ::grpc::ServerContext* context,
    const ::viam::robot::v1::DiscoverComponentsRequest* request,
    ::viam::robot::v1::DiscoverComponentsResponse* response) {
	*response->mutable_discovery() = discovery_response();
	return ::grpc::Status();
}

::grpc::Status TestService::GetOperations(
    ::grpc::ServerContext* context,
    const ::viam::robot::v1::GetOperationsRequest* request,
    ::viam::robot::v1::GetOperationsResponse* response) {
	*response->mutable_operations() = operations_response();
	return ::grpc::Status();
}

TestService service() {
	TestService service;
	// TODO: add extra components to the manager here once they're defined
	// in subsequent features

	return service;
}

// CR erodkin: the question is: how do we generate a stub that calls our
// TestService version of the function specifically that isn't just us defining
// a stub type that serves as an intermediary for a call to the TestService
// function?
int test_resource_names(TestService service) {
	viam::robot::v1::MockRobotServiceStub mock;
	std::shared_ptr<grpc::Channel> c = grpc::CreateChannel(
	    "0.0.0.0:50051", grpc::InsecureChannelCredentials());
	// std::unique_ptr<RobotService::Stub> stub_ = foo.NewStub(c);
	// RobotService::Stub stub_();

	std::unique_ptr<RobotService::Stub> stub_ = RobotService::NewStub(c);
	//  RobotService::StubInterface(service) r;
	//  service.
	//   service.ResourceNames()
	//   grpc::testing::ChannelTestPeer test_channel;
	//   grpc::testing
	//   RobotService::NewStub(grpc::testing::ChannelTestPeer()) stub_;
}
