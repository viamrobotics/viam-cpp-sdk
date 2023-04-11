#pragma once

#include <common/v1/common.pb.h>
#include <component/base/v1/base.grpc.pb.h>

#include <resource/resource_server_base.hpp>
#include <subtype/subtype.hpp>

class BaseServer : public ResourceServerBase,
                   public viam::component::base::v1::BaseService::Service {
   public:
    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) override;
    ::grpc::Status MoveStraight(
        ::grpc::ServerContext* context,
        const ::viam::component::base::v1::MoveStraightRequest* request,
        ::viam::component::base::v1::MoveStraightResponse* response) override;
    ::grpc::Status Spin(::grpc::ServerContext* context,
                        const ::viam::component::base::v1::SpinRequest* request,
                        ::viam::component::base::v1::SpinResponse* response) override;
    ::grpc::Status SetPower(::grpc::ServerContext* context,
                            const ::viam::component::base::v1::SetPowerRequest* request,
                            ::viam::component::base::v1::SetPowerResponse* response) override;
    ::grpc::Status SetVelocity(::grpc::ServerContext* context,
                               const ::viam::component::base::v1::SetVelocityRequest* request,
                               ::viam::component::base::v1::SetVelocityResponse* response) override;
    ::grpc::Status Stop(::grpc::ServerContext* context,
                        const ::viam::component::base::v1::StopRequest* request,
                        ::viam::component::base::v1::StopResponse* response) override;
    ::grpc::Status IsMoving(::grpc::ServerContext* context,
                            const ::viam::component::base::v1::IsMovingRequest* request,
                            ::viam::component::base::v1::IsMovingResponse* response) override;

    void register_server() override;

    std::shared_ptr<SubtypeService> get_sub_svc();

    BaseServer() : sub_svc(std::make_shared<SubtypeService>()){};
    BaseServer(std::shared_ptr<SubtypeService> sub_svc) : sub_svc(sub_svc){};

   private:
    std::shared_ptr<SubtypeService> sub_svc;
};
