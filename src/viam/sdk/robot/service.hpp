/// @file robot/service.cpp
///
/// @brief gRPC service implementation for a `robot`.
#pragma once

#include <string>
#include <thread>
#include <unordered_map>

#include <google/protobuf/struct.pb.h>
#include <grpcpp/support/status.h>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/robot/v1/robot.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

using google::protobuf::RepeatedPtrField;
using viam::robot::v1::Status;

/// @class RobotService_ service.hpp "robot/service.hpp"
/// @brief a gRPC service for a robot.
/// @ingroup Robot
class RobotService_ : public ResourceServer, public viam::robot::v1::RobotService::Service {
   public:
    explicit RobotService_(std::shared_ptr<ResourceManager> manager, Server& server);
    std::shared_ptr<Resource> resource_by_name(Name name);
    ::grpc::Status ResourceNames(::grpc::ServerContext* context,
                                 const ::viam::robot::v1::ResourceNamesRequest* request,
                                 ::viam::robot::v1::ResourceNamesResponse* response) override;
    ::grpc::Status GetStatus(::grpc::ServerContext* context,
                             const ::viam::robot::v1::GetStatusRequest* request,
                             ::viam::robot::v1::GetStatusResponse* response) override;
    ::grpc::Status StreamStatus(
        ::grpc::ServerContext* context,
        const ::viam::robot::v1::StreamStatusRequest* request,
        ::grpc::ServerWriter<::viam::robot::v1::StreamStatusResponse>* writer) override;
    ::grpc::Status StopAll(::grpc::ServerContext* context,
                           const ::viam::robot::v1::StopAllRequest* request,
                           ::viam::robot::v1::StopAllResponse* response) override;

   private:
    std::mutex lock_;
    std::vector<Name> generate_metadata();
    std::vector<status> generate_status(std::vector<Name> resource_names);

    void stream_status(const ::viam::robot::v1::StreamStatusRequest* request,
                       ::grpc::ServerWriter<::viam::robot::v1::StreamStatusResponse>* writer,
                       int interval);
};

}  // namespace sdk
}  // namespace viam
