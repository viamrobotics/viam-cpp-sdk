#ifndef ROBOTSERVICE_H
#define ROBOTSERVICE_H
#include <google/protobuf/struct.pb.h>
#include <grpcpp/support/status.h>

#include <string>
#include <thread>
#include <unordered_map>

#include "../common/utils.h"
#include "../components/component_base.h"
#include "../components/service_base.h"
#include "../robot/client.h"
#include "common/v1/common.pb.h"
#include "grpcpp/server_context.h"
#include "robot/v1/robot.grpc.pb.h"
#include "robot/v1/robot.pb.h"

using google::protobuf::RepeatedPtrField;
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
	    ::grpc::ServerWriter<::viam::robot::v1::StreamStatusResponse>*
		writer) override;
	::grpc::Status StopAll(
	    ::grpc::ServerContext* context,
	    const ::viam::robot::v1::StopAllRequest* request,
	    ::viam::robot::v1::StopAllResponse* response) override;

       private:
	std::vector<ResourceName> generate_metadata();
	std::vector<Status> generate_status(
	    RepeatedPtrField<ResourceName> resources);

	void stream_status(
	    const ::viam::robot::v1::StreamStatusRequest* request,
	    ::grpc::ServerWriter<::viam::robot::v1::StreamStatusResponse>*
		writer,
	    int interval);
};

#endif
