#pragma once

#include <common/v1/common.pb.h>
#include <component/board/v1/board.grpc.pb.h>

#include <resource/resource_server_base.hpp>
#include <subtype/subtype.hpp>

class BoardServer : public ResourceServerBase,
                    public viam::component::board::v1::BoardService::Service {
   public:
    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) override;
    ::grpc::Status Status(::grpc::ServerContext* context,
                          const ::viam::component::board::v1::StatusRequest* request,
                          ::viam::component::board::v1::StatusResponse* response) override;
    ::grpc::Status SetGPIO(::grpc::ServerContext* context,
                           const ::viam::component::board::v1::SetGPIORequest* request,
                           ::viam::component::board::v1::SetGPIOResponse* response) override;
    ::grpc::Status GetGPIO(::grpc::ServerContext* context,
                           const ::viam::component::board::v1::GetGPIORequest* request,
                           ::viam::component::board::v1::GetGPIOResponse* response) override;
    ::grpc::Status PWM(::grpc::ServerContext* context,
                       const ::viam::component::board::v1::PWMRequest* request,
                       ::viam::component::board::v1::PWMResponse* response) override;
    ::grpc::Status SetPWM(::grpc::ServerContext* context,
                          const ::viam::component::board::v1::SetPWMRequest* request,
                          ::viam::component::board::v1::SetPWMResponse* response) override;
    ::grpc::Status PWMFrequency(
        ::grpc::ServerContext* context,
        const ::viam::component::board::v1::PWMFrequencyRequest* request,
        ::viam::component::board::v1::PWMFrequencyResponse* response) override;
    ::grpc::Status SetPWMFrequency(
        ::grpc::ServerContext* context,
        const ::viam::component::board::v1::SetPWMFrequencyRequest* request,
        ::viam::component::board::v1::SetPWMFrequencyResponse* response) override;
    ::grpc::Status ReadAnalogReader(
        ::grpc::ServerContext* context,
        const ::viam::component::board::v1::ReadAnalogReaderRequest* request,
        ::viam::component::board::v1::ReadAnalogReaderResponse* response) override;
    ::grpc::Status GetDigitalInterruptValue(
        ::grpc::ServerContext* context,
        const ::viam::component::board::v1::GetDigitalInterruptValueRequest* request,
        ::viam::component::board::v1::GetDigitalInterruptValueResponse* response) override;
    ::grpc::Status SetPowerMode(
        ::grpc::ServerContext* context,
        const ::viam::component::board::v1::SetPowerModeRequest* request,
        ::viam::component::board::v1::SetPowerModeResponse* response) override;

    void register_server() override;

    std::shared_ptr<SubtypeService> get_sub_svc();

    BoardServer() : sub_svc(std::make_shared<SubtypeService>()){};
    BoardServer(std::shared_ptr<SubtypeService> sub_svc) : sub_svc(sub_svc){};

   private:
    std::shared_ptr<SubtypeService> sub_svc;
};
