#pragma once

#include <string>
#include <thread>
#include <unordered_map>

#include <google/protobuf/struct.pb.h>
#include <grpcpp/support/status.h>

#include <common/v1/common.pb.h>
#include <robot/v1/robot.grpc.pb.h>
#include <robot/v1/robot.pb.h>

#include <common/utils.hpp>
#include <components/component_base.hpp>
#include <components/service_base.hpp>
#include <resource/resource.hpp>
#include <robot/client.hpp>

namespace viam {
namespace cppsdk {

using google::protobuf::RepeatedPtrField;
using viam::common::v1::ResourceName;
using viam::robot::v1::Status;

class RobotService_ : public ComponentServiceBase, public viam::robot::v1::RobotService::Service {
   public:
    RobotService_();
    static std::shared_ptr<RobotService_> create();
    std::shared_ptr<ResourceBase> resource_by_name(Name name);
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
    std::mutex lock;
    std::vector<ResourceName> generate_metadata();
    std::vector<Status> generate_status(RepeatedPtrField<ResourceName> resources);

    void stream_status(const ::viam::robot::v1::StreamStatusRequest* request,
                       ::grpc::ServerWriter<::viam::robot::v1::StreamStatusResponse>* writer,
                       int interval);
};

}  // namespace cppsdk
}  // namespace viam
