#pragma once

#include <common/v1/common.pb.h>
#include <robot/v1/robot.grpc.pb.h>
#include <robot/v1/robot.pb.h>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/services/framesystem.hpp>

namespace viam {
namespace sdktests {
namespace robot {

using namespace viam::sdk;
// Ideally we wouldn't have to mock out a service at all and could just use the standard
// RobotService, but unfortunately that service doesn't implement all of the gRPC methods,
// e.g. `FrameSystemConfig` is not implemented.
//
// By mocking just those methods that are unimplemented but used by a client, and otherwise
// inheriting from the actual Service, we can reliably and fully test both the client and
// service functionality. However, if/when these methods are updated in the actual RobotService_,
// we should update the tests here accordingly.
//
class MockRobotService : public ResourceServer, public viam::robot::v1::RobotService::Service {
   public:
    MockRobotService(const std::shared_ptr<ResourceManager>& manager, Server& server);

    std::shared_ptr<Resource> resource_by_name(const Name& name);

    ::grpc::Status ResourceNames(::grpc::ServerContext* context,
                                 const ::viam::robot::v1::ResourceNamesRequest* request,
                                 ::viam::robot::v1::ResourceNamesResponse* response) override;

    ::grpc::Status StopAll(::grpc::ServerContext* context,
                           const ::viam::robot::v1::StopAllRequest* request,
                           ::viam::robot::v1::StopAllResponse* response) override;

    ::grpc::Status FrameSystemConfig(
        ::grpc::ServerContext* context,
        const ::viam::robot::v1::FrameSystemConfigRequest* request,
        ::viam::robot::v1::FrameSystemConfigResponse* response) override;

    ::grpc::Status TransformPose(::grpc::ServerContext* context,
                                 const ::viam::robot::v1::TransformPoseRequest* request,
                                 ::viam::robot::v1::TransformPoseResponse* response) override;

    ::grpc::Status TransformPCD(::grpc::ServerContext* context,
                                const ::viam::robot::v1::TransformPCDRequest* request,
                                ::viam::robot::v1::TransformPCDResponse* response) override;

    ::grpc::Status GetPose(::grpc::ServerContext* context,
                           const ::viam::robot::v1::GetPoseRequest* request,
                           ::viam::robot::v1::GetPoseResponse* response) override;

    // The most recent request each of these RPCs received, so tests can check what the client put
    // on the wire and not just how it decoded the canned response.
    ::viam::robot::v1::GetPoseRequest last_get_pose_request();
    ::viam::robot::v1::TransformPCDRequest last_transform_pcd_request();

    ::grpc::Status GetMachineStatus(::grpc::ServerContext* context,
                                    const ::viam::robot::v1::GetMachineStatusRequest* request,
                                    ::viam::robot::v1::GetMachineStatusResponse* response) override;

    ::grpc::Status GetOperations(::grpc::ServerContext* context,
                                 const ::viam::robot::v1::GetOperationsRequest* request,
                                 ::viam::robot::v1::GetOperationsResponse* response) override;

    // A module connected to this mock forwards its logs here. Accepting them keeps the module
    // from complaining on the console about every line it could not deliver.
    ::grpc::Status Log(::grpc::ServerContext* context,
                       const ::viam::robot::v1::LogRequest* request,
                       ::viam::robot::v1::LogResponse* response) override;

   private:
    std::mutex lock_;
    ::viam::robot::v1::GetPoseRequest last_get_pose_request_;
    ::viam::robot::v1::TransformPCDRequest last_transform_pcd_request_;
    std::vector<common::v1::ResourceName> generate_metadata_();
};

pose default_pose(int offset = 0);
std::vector<RobotClient::operation> mock_operations_response();
std::vector<viam::robot::v1::Operation> mock_proto_operations_response();
std::vector<Name> mock_resource_names_response();
std::vector<common::v1::ResourceName> mock_proto_resource_names_response();
std::vector<FrameSystem::frame_system_config> mock_config_response();
std::vector<viam::robot::v1::FrameSystemConfig> mock_proto_config_response();
pose_in_frame mock_transform_response();
common::v1::PoseInFrame mock_proto_transform_response();
pose_in_frame mock_get_pose_response(const std::string& destination_frame);
common::v1::PoseInFrame mock_proto_get_pose_response(const std::string& destination_frame);
std::vector<unsigned char> mock_transform_pcd_response();
std::string mock_proto_transform_pcd_response();
RobotClient::machine_status mock_machine_status_response();
viam::robot::v1::GetMachineStatusResponse mock_proto_machine_status_response();

}  // namespace robot
}  // namespace sdktests
}  // namespace viam
