#include <viam/sdk/components/private/board_server.hpp>

#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/board.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {
namespace impl {

BoardServer::BoardServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {};

::grpc::Status BoardServer::SetGPIO(::grpc::ServerContext*,
                                    const ::viam::component::board::v1::SetGPIORequest* request,
                                    ::viam::component::board::v1::SetGPIOResponse*) noexcept {
    return make_service_helper<Board>(
        "BoardServer::SetGPIO", this, request)([&](auto& helper, auto& board) {
        board->set_gpio(request->pin(), request->high(), helper.getExtra());
    });
}

::grpc::Status BoardServer::GetGPIO(
    ::grpc::ServerContext*,
    const ::viam::component::board::v1::GetGPIORequest* request,
    ::viam::component::board::v1::GetGPIOResponse* response) noexcept {
    return make_service_helper<Board>(
        "BoardServer::GetGPIO", this, request)([&](auto& helper, auto& board) {
        response->set_high(board->get_gpio(request->pin(), helper.getExtra()));
    });
}

::grpc::Status BoardServer::PWM(::grpc::ServerContext*,
                                const ::viam::component::board::v1::PWMRequest* request,
                                ::viam::component::board::v1::PWMResponse* response) noexcept {
    return make_service_helper<Board>(
        "BoardServer::PWM", this, request)([&](auto& helper, auto& board) {
        response->set_duty_cycle_pct(board->get_pwm_duty_cycle(request->pin(), helper.getExtra()));
    });
}

::grpc::Status BoardServer::SetPWM(::grpc::ServerContext*,
                                   const ::viam::component::board::v1::SetPWMRequest* request,
                                   ::viam::component::board::v1::SetPWMResponse*) noexcept {
    return make_service_helper<Board>(
        "BoardServer::SetPWM", this, request)([&](auto& helper, auto& board) {
        board->set_pwm_duty_cycle(request->pin(), request->duty_cycle_pct(), helper.getExtra());
    });
}

::grpc::Status BoardServer::PWMFrequency(
    ::grpc::ServerContext*,
    const ::viam::component::board::v1::PWMFrequencyRequest* request,
    ::viam::component::board::v1::PWMFrequencyResponse* response) noexcept {
    return make_service_helper<Board>(
        "BoardServer::PWMFrequency", this, request)([&](auto& helper, auto& board) {
        const uint64_t result = board->get_pwm_frequency(request->pin(), helper.getExtra());
        response->set_frequency_hz(result);
    });
}

::grpc::Status BoardServer::SetPWMFrequency(
    ::grpc::ServerContext*,
    const ::viam::component::board::v1::SetPWMFrequencyRequest* request,
    ::viam::component::board::v1::SetPWMFrequencyResponse*) noexcept {
    return make_service_helper<Board>(
        "BoardServer::SetPWMFrequency", this, request)([&](auto& helper, auto& board) {
        board->set_pwm_frequency(request->pin(), request->frequency_hz(), helper.getExtra());
    });
}

::grpc::Status BoardServer::DoCommand(grpc::ServerContext*,
                                      const viam::common::v1::DoCommandRequest* request,
                                      viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Board>(
        "BoardServer::DoCommand", this, request)([&](auto&, auto& board) {
        const ProtoStruct result = board->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
}

::grpc::Status BoardServer::ReadAnalogReader(
    ::grpc::ServerContext*,
    const ::viam::component::board::v1::ReadAnalogReaderRequest* request,
    ::viam::component::board::v1::ReadAnalogReaderResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Board::ReadAnalogReader] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->board_name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->board_name());
    }

    const std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    ProtoStruct extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    const Board::analog_response result = board->read_analog(request->analog_reader_name(), extra);
    response->set_value(result.value);
    response->set_min_range(result.min_range);
    response->set_max_range(result.max_range);
    response->set_step_size(result.step_size);

    return ::grpc::Status();
}

::grpc::Status BoardServer::WriteAnalog(
    ::grpc::ServerContext*,
    const ::viam::component::board::v1::WriteAnalogRequest* request,
    ::viam::component::board::v1::WriteAnalogResponse*) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Board::WriteAnalog] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    ProtoStruct extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    board->write_analog(request->pin(), request->value(), extra);
    return ::grpc::Status();
}

::grpc::Status BoardServer::GetDigitalInterruptValue(
    ::grpc::ServerContext*,
    const ::viam::component::board::v1::GetDigitalInterruptValueRequest* request,
    ::viam::component::board::v1::GetDigitalInterruptValueResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Board::GetDigitalInterruptValue] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->board_name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->board_name());
    }

    const std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    ProtoStruct extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    const Board::digital_value result =
        board->read_digital_interrupt(request->digital_interrupt_name(), extra);
    response->set_value(result);

    return ::grpc::Status();
}

::grpc::Status BoardServer::StreamTicks(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::StreamTicksRequest* request,
    ::grpc::ServerWriter<::viam::component::board::v1::StreamTicksResponse>* writer) noexcept {
    make_service_helper<Board>(
        "BoardServer::StreamTicks", this, request)([&](auto& helper, auto& board) {
        const std::vector<std::string> digital_interrupt_names(request->pin_names().begin(),
                                                               request->pin_names().end());
        auto writeTick = [writer, context](Board::Tick&& tick) {
            if (context->IsCancelled()) {
                // send bool to tell the board to stop calling the callback function.
                return false;
            }
            ::viam::component::board::v1::StreamTicksResponse response;
            response.set_pin_name(std::move(tick.pin_name));
            response.set_high(std::move(tick.high));
            response.set_time(std::move(tick.time.count()));
            writer->Write(response);
            return true;
        };
        board->stream_ticks(digital_interrupt_names, writeTick, helper.getExtra());
    });

    return ::grpc::Status();
}

::grpc::Status BoardServer::SetPowerMode(
    ::grpc::ServerContext*,
    const ::viam::component::board::v1::SetPowerModeRequest* request,
    ::viam::component::board::v1::SetPowerModeResponse*) noexcept {
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
    ::grpc::ServerContext*,
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

}  // namespace impl
}  // namespace sdk
}  // namespace viam
