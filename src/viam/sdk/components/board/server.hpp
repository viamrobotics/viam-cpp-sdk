/// @file components/board/server.hpp
///
/// @brief Implements a gRPC server for the `Board` component.
#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/board/v1/board.grpc.pb.h>

#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {

/// @class BoardServer
/// @brief gRPC server implementation of a `Board` component.
/// @ingroup Board
class BoardServer : public ResourceServer,
                    public viam::component::board::v1::BoardService::Service {
   public:
    BoardServer();
    explicit BoardServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status Status(::grpc::ServerContext* context,
                          const ::viam::component::board::v1::StatusRequest* request,
                          ::viam::component::board::v1::StatusResponse* response) noexcept override;

    ::grpc::Status SetGPIO(
        ::grpc::ServerContext* context,
        const ::viam::component::board::v1::SetGPIORequest* request,
        ::viam::component::board::v1::SetGPIOResponse* response) noexcept override;

    ::grpc::Status GetGPIO(
        ::grpc::ServerContext* context,
        const ::viam::component::board::v1::GetGPIORequest* request,
        ::viam::component::board::v1::GetGPIOResponse* response) noexcept override;

    ::grpc::Status PWM(::grpc::ServerContext* context,
                       const ::viam::component::board::v1::PWMRequest* request,
                       ::viam::component::board::v1::PWMResponse* response) noexcept override;

    ::grpc::Status SetPWM(::grpc::ServerContext* context,
                          const ::viam::component::board::v1::SetPWMRequest* request,
                          ::viam::component::board::v1::SetPWMResponse* response) noexcept override;

    ::grpc::Status PWMFrequency(
        ::grpc::ServerContext* context,
        const ::viam::component::board::v1::PWMFrequencyRequest* request,
        ::viam::component::board::v1::PWMFrequencyResponse* response) noexcept override;

    ::grpc::Status SetPWMFrequency(
        ::grpc::ServerContext* context,
        const ::viam::component::board::v1::SetPWMFrequencyRequest* request,
        ::viam::component::board::v1::SetPWMFrequencyResponse* response) noexcept override;

    ::grpc::Status DoCommand(grpc::ServerContext* context,
                             const viam::common::v1::DoCommandRequest* request,
                             viam::common::v1::DoCommandResponse* response) noexcept override;

    ::grpc::Status ReadAnalogReader(
        ::grpc::ServerContext* context,
        const ::viam::component::board::v1::ReadAnalogReaderRequest* request,
        ::viam::component::board::v1::ReadAnalogReaderResponse* response) override;

    ::grpc::Status WriteAnalog(::grpc::ServerContext* context,
                               const component::board::v1::WriteAnalogRequest* request,
                               component::board::v1::WriteAnalogResponse* response) override;

    ::grpc::Status GetDigitalInterruptValue(
        ::grpc::ServerContext* context,
        const ::viam::component::board::v1::GetDigitalInterruptValueRequest* request,
        ::viam::component::board::v1::GetDigitalInterruptValueResponse* response) override;

    ::grpc::Status SetPowerMode(
        ::grpc::ServerContext* context,
        const ::viam::component::board::v1::SetPowerModeRequest* request,
        ::viam::component::board::v1::SetPowerModeResponse* response) noexcept override;

    ::grpc::Status GetGeometries(
        ::grpc::ServerContext* context,
        const ::viam::common::v1::GetGeometriesRequest* request,
        ::viam::common::v1::GetGeometriesResponse* response) noexcept override;

    void register_server(std::shared_ptr<Server> server) override;
};

}  // namespace sdk
}  // namespace viam
