/// @file components/motor/server.hpp
///
/// @brief Implements a gRPC server for the `Motor` component.
#pragma once

#include <common/v1/common.pb.h>
#include <component/motor/v1/motor.grpc.pb.h>

#include <resource/resource_server_base.hpp>
#include <subtype/subtype.hpp>

/// @class MotorServer
/// @brief gRPC server implementation of a `Motor` component.
/// @ingroup Motor
class MotorServer : public ResourceServerBase,
                     public viam::component::motor::v1::MotorService::Service {
   public:
   
    ::grpc::Status SetPower(::grpc::ServerContext* context,
                            const ::viam::component::motor::v1::SetPowerRequest* request,
                            ::viam::component::motor::v1::SetPowerResponse* response) override;
   
    ::grpc::Status GoFor(::grpc::ServerContext* context,
                            const ::viam::component::motor::v1::GoForRequest* request,
                            ::viam::component::motor::v1::GoForResponse* response) override;
   
    ::grpc::Status GoTo(::grpc::ServerContext* context,
                            const ::viam::component::motor::v1::GoToRequest* request,
                            ::viam::component::motor::v1::GoToResponse* response) override;
   
    ::grpc::Status ResetZeroPosition(::grpc::ServerContext* context,
                            const ::viam::component::motor::v1::ResetZeroPositionRequest* request,
                            ::viam::component::motor::v1::ResetZeroPositionResponse* response) override;
   
    ::grpc::Status GetPosition(::grpc::ServerContext* context,
                            const ::viam::component::motor::v1::GetPositionRequest* request,
                            ::viam::component::motor::v1::GetPositionResponse* response) override;
   
    ::grpc::Status GetProperties(::grpc::ServerContext* context,
                            const ::viam::component::motor::v1::GetPropertiesRequest* request,
                            ::viam::component::motor::v1::GetPropertiesResponse* response) override;
   
    ::grpc::Status Stop(::grpc::ServerContext* context,
                            const ::viam::component::motor::v1::StopRequest* request,
                            ::viam::component::motor::v1::StopResponse* response) override;
   
    ::grpc::Status IsPowered(::grpc::ServerContext* context,
                            const ::viam::component::motor::v1::IsPoweredRequest* request,
                            ::viam::component::motor::v1::IsPoweredResponse* response) override;
   
    ::grpc::Status IsMoving(::grpc::ServerContext* context,
                            const ::viam::component::motor::v1::IsMovingRequest* request,
                            ::viam::component::motor::v1::IsMovingResponse* response) override;
   
    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                            const ::viam::component::motor::v1::common.v1.DoCommandRequest* request,
                            ::viam::component::motor::v1::common.v1.DoCommandResponse* response) override;
   

    void register_server(std::shared_ptr<Server> server) override;

    std::shared_ptr<SubtypeService> get_sub_svc();

    MotorServer() : sub_svc(std::make_shared<SubtypeService>()){};
    MotorServer(std::shared_ptr<SubtypeService> sub_svc) : sub_svc(sub_svc){};

   private:
    std::shared_ptr<SubtypeService> sub_svc;
};
