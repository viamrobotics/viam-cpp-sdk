#include <robot/v1/robot.pb.h>

#include <viam/sdk/robot/service.hpp>

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
class MockRobotService : public RobotService_ {
   public:
    MockRobotService(std::shared_ptr<Server> server) : RobotService_(server){};

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
};

std::vector<viam::robot::v1::Operation> mock_operations_response();
std::vector<viam::robot::v1::Discovery> mock_discovery_response();
std::vector<ResourceName> mock_resource_names_response();
std::vector<FrameSystemConfig> mock_config_response();
common::v1::PoseInFrame mock_transform_response();
std::vector<Status> mock_status_response();

}  // namespace robot
}  // namespace sdktests
}  // namespace viam
