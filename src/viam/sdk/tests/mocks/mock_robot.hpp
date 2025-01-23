#pragma once

#include <common/v1/common.pb.h>
#include <robot/v1/robot.grpc.pb.h>
#include <robot/v1/robot.pb.h>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/rpc/server.hpp>

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

    ::grpc::Status GetOperations(::grpc::ServerContext* context,
                                 const ::viam::robot::v1::GetOperationsRequest* request,
                                 ::viam::robot::v1::GetOperationsResponse* response) override;

   private:
    std::mutex lock_;
    std::vector<common::v1::ResourceName> generate_metadata_();
};

pose default_pose(int offset = 0);
std::vector<RobotClient::operation> mock_operations_response();
std::vector<viam::robot::v1::Operation> mock_proto_operations_response();
std::vector<Name> mock_resource_names_response();
std::vector<common::v1::ResourceName> mock_proto_resource_names_response();
std::vector<RobotClient::frame_system_config> mock_config_response();
std::vector<viam::robot::v1::FrameSystemConfig> mock_proto_config_response();
pose_in_frame mock_transform_response();
common::v1::PoseInFrame mock_proto_transform_response();

}  // namespace robot
}  // namespace sdktests
}  // namespace viam
