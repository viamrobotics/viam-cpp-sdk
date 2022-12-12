#include <grpcpp/support/status.h>

#include "../components/service_base.h"
#include "../robot/client.h"
#include "common/v1/common.pb.h"
#include "grpcpp/server_context.h"
#include "robot/v1/robot.grpc.pb.h"
#include "robot/v1/robot.pb.h"

using viam::common::v1::ResourceName;
using viam::robot::v1::Status;

class RobotService_ : public ComponentServiceBase,
		      public viam::robot::v1::RobotService::Service {
       public:
	::grpc::Status ResourceNames(
	    ::grpc::ServerContext* context,
	    const ::viam::robot::v1::ResourceNamesRequest* request,
	    ::viam::robot::v1::ResourceNamesResponse* response) override;
	::grpc::Status GetStatus(
	    ::grpc::ServerContext* context,
	    const ::viam::robot::v1::GetStatusRequest* request,
	    ::viam::robot::v1::GetStatusResponse* response) override;
	::grpc::Status StreamStatus(
	    ::grpc::ServerContext* context,
	    const ::viam::robot::v1::StreamStatusRequest* request,
	    ::grpc::ServerWriter< ::viam::robot::v1::StreamStatusResponse>*
		writer) override;
	::grpc::Status GetOperations(
	    ::grpc::ServerContext* context,
	    const ::viam::robot::v1::GetOperationsRequest* request,
	    ::viam::robot::v1::GetOperationsResponse* response) override;

	::grpc::Status ResourceRPCSubtypes(
	    ::grpc::ServerContext* context,
	    const ::viam::robot::v1::ResourceRPCSubtypesRequest* request,
	    ::viam::robot::v1::ResourceRPCSubtypesResponse* response) override;
	::grpc::Status CancelOperation(
	    ::grpc::ServerContext* context,
	    const ::viam::robot::v1::CancelOperationRequest* request,
	    ::viam::robot::v1::CancelOperationResponse* response) override;
	::grpc::Status BlockForOperation(
	    ::grpc::ServerContext* context,
	    const ::viam::robot::v1::BlockForOperationRequest* request,
	    ::viam::robot::v1::BlockForOperationResponse* response) override;
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
	::grpc::Status StopAll(
	    ::grpc::ServerContext* context,
	    const ::viam::robot::v1::StopAllRequest* request,
	    ::viam::robot::v1::StopAllResponse* response) override;

       private:
	std::vector<ResourceName> generate_metadata();
	std::vector<Status> generate_status();
};

std::vector<ResourceName> RobotService_::generate_metadata() {
	std::vector<ResourceName> metadata;
	for (const auto& key_and_val : manager.components) {
		for (ResourceName resource :
		     resource_names_for_component(key_and_val.second)) {
			metadata.push_back(resource);
		}
	}
	return metadata;
}

::grpc::Status RobotService_::ResourceNames(
    ::grpc::ServerContext* context,
    const viam::robot::v1::ResourceNamesRequest* request,
    viam::robot::v1::ResourceNamesResponse* response) {}
