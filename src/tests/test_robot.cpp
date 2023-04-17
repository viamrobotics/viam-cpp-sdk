#include <memory>
#include <utility>

#include <google/protobuf/struct.pb.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/support/stub_options.h>

#include <common/v1/common.pb.h>
#include <component/arm/v1/arm.grpc.pb.h>
#include <component/arm/v1/arm.pb.h>
#include <robot/v1/robot.grpc.pb.h>
#include <robot/v1/robot.pb.h>

#include <common/utils.hpp>
#include <robot/service.hpp>

namespace viam {
namespace cppsdktests {

using namespace viam::cppsdk;

using google::protobuf::RepeatedPtrField;
using viam::common::v1::PoseInFrame;
using viam::common::v1::ResourceName;
class TestService : public RobotService_ {
   public:
    ::grpc::Status FrameSystemConfig(
        ::grpc::ServerContext* context,
        const ::viam::robot::v1::FrameSystemConfigRequest* request,
        ::viam::robot::v1::FrameSystemConfigResponse* response) override;

    ::grpc::Status TransformPose(::grpc::ServerContext* context,
                                 const ::viam::robot::v1::TransformPoseRequest* request,
                                 ::viam::robot::v1::TransformPoseResponse* response) override;

    ::grpc::Status DiscoverComponents(
        ::grpc::ServerContext* context,
        const ::viam::robot::v1::DiscoverComponentsRequest* request,
        ::viam::robot::v1::DiscoverComponentsResponse* response) override;

    ::grpc::Status GetOperations(::grpc::ServerContext* context,
                                 const ::viam::robot::v1::GetOperationsRequest* request,
                                 ::viam::robot::v1::GetOperationsResponse* response) override;

    TestService();
};

// TODO(RSDK-1629): when we flesh out the tests here, move MockStub into its own
// file.
class MockStub : public RobotService::StubInterface {
   public:
    TestService service;
    MockStub(TestService service) {
        service = service;
    };

    ::grpc::Status ResourceNames(::grpc::ClientContext* context,
                                 const ::viam::robot::v1::ResourceNamesRequest& request,
                                 ::viam::robot::v1::ResourceNamesResponse* response) override {
        grpc::ServerContext* ctx;
        return service.ResourceNames(ctx, &request, response);
    }

    ::grpc::Status GetStatus(::grpc::ClientContext* context,
                             const ::viam::robot::v1::GetStatusRequest& request,
                             ::viam::robot::v1::GetStatusResponse* response) override {
        grpc::ServerContext* ctx;
        return service.GetStatus(ctx, &request, response);
    }

    ::grpc::Status GetOperations(::grpc::ClientContext* context,
                                 const ::viam::robot::v1::GetOperationsRequest& request,
                                 ::viam::robot::v1::GetOperationsResponse* response) override;

    ::grpc::Status GetSessions(::grpc::ClientContext* context,
                               const ::viam::robot::v1::GetSessionsRequest& request,
                               ::viam::robot::v1::GetSessionsResponse* response) override;

    ::grpc::Status ResourceRPCSubtypes(
        ::grpc::ClientContext* context,
        const ::viam::robot::v1::ResourceRPCSubtypesRequest& request,
        ::viam::robot::v1::ResourceRPCSubtypesResponse* response) override;

    ::grpc::Status CancelOperation(::grpc::ClientContext* context,
                                   const ::viam::robot::v1::CancelOperationRequest& request,
                                   ::viam::robot::v1::CancelOperationResponse* response) override;

    ::grpc::Status BlockForOperation(
        ::grpc::ClientContext* context,
        const ::viam::robot::v1::BlockForOperationRequest& request,
        ::viam::robot::v1::BlockForOperationResponse* response) override;

    ::grpc::Status DiscoverComponents(
        ::grpc::ClientContext* context,
        const ::viam::robot::v1::DiscoverComponentsRequest& request,
        ::viam::robot::v1::DiscoverComponentsResponse* response) override;

    ::grpc::Status FrameSystemConfig(
        ::grpc::ClientContext* context,
        const ::viam::robot::v1::FrameSystemConfigRequest& request,
        ::viam::robot::v1::FrameSystemConfigResponse* response) override;

    ::grpc::Status TransformPose(::grpc::ClientContext* context,
                                 const ::viam::robot::v1::TransformPoseRequest& request,
                                 ::viam::robot::v1::TransformPoseResponse* response) override;

    ::grpc::Status StopAll(::grpc::ClientContext* context,
                           const ::viam::robot::v1::StopAllRequest& request,
                           ::viam::robot::v1::StopAllResponse* response) override;

    ::grpc::Status StartSession(::grpc::ClientContext* context,
                                const ::viam::robot::v1::StartSessionRequest& request,
                                ::viam::robot::v1::StartSessionResponse* response) override;

    ::grpc::Status SendSessionHeartbeat(
        ::grpc::ClientContext* context,
        const ::viam::robot::v1::SendSessionHeartbeatRequest& request,
        ::viam::robot::v1::SendSessionHeartbeatResponse* response) override;

    class async_interface* async() override;
};

std::vector<ResourceName> names_for_testing() {
    std::vector<ResourceName> vec;
    ResourceName arm1;
    *arm1.mutable_namespace_() = RDK;
    *arm1.mutable_type() = COMPONENT;
    *arm1.mutable_name() = "arm";
    *arm1.mutable_subtype() = "arm1";

    ResourceName camera1;
    *camera1.mutable_namespace_() = RDK;
    *camera1.mutable_type() = COMPONENT;
    *camera1.mutable_name() = "camera";
    *camera1.mutable_subtype() = "camera1";

    ResourceName motor1;
    *motor1.mutable_namespace_() = RDK;
    *motor1.mutable_type() = COMPONENT;
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
    google::protobuf::MapPair<std::string, google::protobuf::Value> str_pair("foo", str);

    google::protobuf::Value i;
    i.set_number_value(1);
    google::protobuf::MapPair<std::string, google::protobuf::Value> int_pair("one", i);

    google::protobuf::Map<std::string, google::protobuf::Value>* map = results.mutable_fields();
    map->insert(str_pair);
    map->insert(int_pair);

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

::grpc::Status TestService::TransformPose(::grpc::ServerContext* context,
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

::grpc::Status TestService::GetOperations(::grpc::ServerContext* context,
                                          const ::viam::robot::v1::GetOperationsRequest* request,
                                          ::viam::robot::v1::GetOperationsResponse* response) {
    *response->mutable_operations() = operations_response();
    return ::grpc::Status();
}

TestService service() {
    TestService service;
    // TODO(RSDK-1629): add extra components to the manager here once
    // they're defined in subsequent features

    return service;
}

int test_resource_names(TestService service) {
    MockStub mock_(service);
    grpc::ClientContext ctx;
    viam::robot::v1::ResourceNamesRequest req;
    viam::robot::v1::ResourceNamesResponse resp;
    grpc::Status status = mock_.ResourceNames(&ctx, req, &resp);

    RepeatedPtrField<ResourceName> resources = resp.resources();
    std::vector<ResourceName> from_call;
    for (auto& resource : resources) {
        from_call.push_back(resource);
    }

    if (from_call == names_for_testing()) {
        return 0;
    }
    return 1;
}

}  // namespace cppsdktests
}  // namespace viam
