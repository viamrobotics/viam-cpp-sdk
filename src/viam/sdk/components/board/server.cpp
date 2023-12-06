#include <viam/sdk/components/board/server.hpp>

#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/board/board.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

BoardServer::BoardServer() : ResourceServer(std::make_shared<ResourceManager>()){};
BoardServer::BoardServer(std::shared_ptr<ResourceManager> manager) : ResourceServer(manager){};

::grpc::Status BoardServer::Status(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::StatusRequest* request,
    ::viam::component::board::v1::StatusResponse* response) noexcept {
    return make_service_helper<Board>(
        "BoardServer::Status", this, request)([&](auto& helper, auto& board) {
        const viam::common::v1::BoardStatus status =
            Board::to_proto(board->get_status(helper.getExtra()));
        *response->mutable_status() = status;
    });
}

::grpc::Status BoardServer::SetGPIO(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::SetGPIORequest* request,
    ::viam::component::board::v1::SetGPIOResponse* response) noexcept {
    return make_service_helper<Board>(
        "BoardServer::SetGPIO", this, request)([&](auto& helper, auto& board) {
        board->set_gpio(request->pin(), request->high(), helper.getExtra());
    });
}

::grpc::Status BoardServer::GetGPIO(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::GetGPIORequest* request,
    ::viam::component::board::v1::GetGPIOResponse* response) noexcept {
    return make_service_helper<Board>(
        "BoardServer::GetGPIO", this, request)([&](auto& helper, auto& board) {
        response->set_high(board->get_gpio(request->pin(), helper.getExtra()));
    });
}

::grpc::Status BoardServer::PWM(::grpc::ServerContext* context,
                                const ::viam::component::board::v1::PWMRequest* request,
                                ::viam::component::board::v1::PWMResponse* response) noexcept {
    return make_service_helper<Board>(
        "BoardServer::PWM", this, request)([&](auto& helper, auto& board) {
        response->set_duty_cycle_pct(board->get_pwm_duty_cycle(request->pin(), helper.getExtra()));
    });
}

::grpc::Status BoardServer::SetPWM(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::SetPWMRequest* request,
    ::viam::component::board::v1::SetPWMResponse* response) noexcept {
    return make_service_helper<Board>(
        "BoardServer::SetPWM", this, request)([&](auto& helper, auto& board) {
        board->set_pwm_duty_cycle(request->pin(), request->duty_cycle_pct(), helper.getExtra());
    });
}

::grpc::Status BoardServer::PWMFrequency(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::PWMFrequencyRequest* request,
    ::viam::component::board::v1::PWMFrequencyResponse* response) noexcept {
    return make_service_helper<Board>(
        "BoardServer::PWMFrequency", this, request)([&](auto& helper, auto& board) {
        const uint64_t result = board->get_pwm_frequency(request->pin(), helper.getExtra());
        response->set_frequency_hz(result);
    });
}

::grpc::Status BoardServer::SetPWMFrequency(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::SetPWMFrequencyRequest* request,
    ::viam::component::board::v1::SetPWMFrequencyResponse* response) noexcept {
    return make_service_helper<Board>(
        "BoardServer::SetPWMFrequency", this, request)([&](auto& helper, auto& board) {
        board->set_pwm_frequency(request->pin(), request->frequency_hz(), helper.getExtra());
    });
}

::grpc::Status BoardServer::DoCommand(grpc::ServerContext* context,
                                      const viam::common::v1::DoCommandRequest* request,
                                      viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Board>(
        "BoardServer::DoCommand", this, request)([&](auto& helper, auto& board) {
        const AttributeMap result = board->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
}

::grpc::Status BoardServer::ReadAnalogReader(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::ReadAnalogReaderRequest* request,
    ::viam::component::board::v1::ReadAnalogReaderResponse* response) noexcept {
    return make_service_helper<Board>(
        "BoardServer::ReadAnalogReader", this, request)([&](auto& helper, auto& board) {
        const Board::analog_value result =
            board->read_analog(request->analog_reader_name(), helper.getExtra());
        response->set_value(result);
    });
}

::grpc::Status BoardServer::GetDigitalInterruptValue(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::GetDigitalInterruptValueRequest* request,
    ::viam::component::board::v1::GetDigitalInterruptValueResponse* response) noexcept {
    return make_service_helper<Board>(
        "BoardServer::GetDigitalInterruptValue", this, request)([&](auto& helper, auto& board) {
        const Board::digital_value result =
            board->read_digital_interrupt(request->digital_interrupt_name(), helper.getExtra());
        response->set_value(result);
    });
}

::grpc::Status BoardServer::SetPowerMode(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::SetPowerModeRequest* request,
    ::viam::component::board::v1::SetPowerModeResponse* response) noexcept {
    return make_service_helper<Board>(
        "BoardServer::SetPowerMode", this, request)([&](auto& helper, auto& board) {
        if (request->has_duration()) {
            auto duration = ::viam::sdk::from_proto(request->duration());
            board->set_power_mode(
                Board::from_proto(request->power_mode()), helper.getExtra(), duration);
        } else {
            board->set_power_mode(Board::from_proto(request->power_mode()), helper.getExtra());
        }
    });
}

::grpc::Status BoardServer::GetGeometries(
    ::grpc::ServerContext* context,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<Board>(
        "BoardServer::GetGeometries", this, request)([&](auto& helper, auto& board) {
        const std::vector<GeometryConfig> geometries = board->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = geometry.to_proto();
        }
    });
}

void BoardServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

}  // namespace sdk
}  // namespace viam
