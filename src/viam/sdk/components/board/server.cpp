#include <viam/sdk/components/board/server.hpp>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/board/board.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

::grpc::Status BoardServer::Status(::grpc::ServerContext* context,
                                   const ::viam::component::board::v1::StatusRequest* request,
                                   ::viam::component::board::v1::StatusResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Board::Status] without a request");
    };

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    Board::status result = board->status();
    response->set_status(result.status);

    return ::grpc::Status();
}

::grpc::Status BoardServer::SetGPIO(::grpc::ServerContext* context,
                                    const ::viam::component::board::v1::SetGPIORequest* request,
                                    ::viam::component::board::v1::SetGPIOResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Board::SetGPIO] without a request");
    };

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    board->set_gpio(request->pin(), request->high());

    return ::grpc::Status();
}

::grpc::Status BoardServer::GetGPIO(::grpc::ServerContext* context,
                                    const ::viam::component::board::v1::GetGPIORequest* request,
                                    ::viam::component::board::v1::GetGPIOResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Board::GetGPIO] without a request");
    };

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    Board::gpio_pin result = board->get_gpio(request->pin());
    response->set_high(result.high);

    return ::grpc::Status();
}

::grpc::Status BoardServer::PWM(::grpc::ServerContext* context,
                                const ::viam::component::board::v1::PWMRequest* request,
                                ::viam::component::board::v1::PWMResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Board::PWM] without a request");
    };

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    Board::duty_cycle result = board->get_pwm(request->pin());
    response->set_duty_cycle_pct(result.duty_cycle_pct);

    return ::grpc::Status();
}

::grpc::Status BoardServer::SetPWM(::grpc::ServerContext* context,
                                   const ::viam::component::board::v1::SetPWMRequest* request,
                                   ::viam::component::board::v1::SetPWMResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Board::SetPWM] without a request");
    };

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    board->set_pwm(request->pin(), request->duty_cycle_pct());

    return ::grpc::Status();
}

::grpc::Status BoardServer::PWMFrequency(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::PWMFrequencyRequest* request,
    ::viam::component::board::v1::PWMFrequencyResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Board::PWMFrequency] without a request");
    };

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    Board::pwm_frequency result = board->get_pwm_frequency(request->pin());
    response->set_frequency_hz(result.frequency_hz);

    return ::grpc::Status();
}

::grpc::Status BoardServer::SetPWMFrequency(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::SetPWMFrequencyRequest* request,
    ::viam::component::board::v1::SetPWMFrequencyResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Board::SetPWMFrequency] without a request");
    };

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    board->set_pwm_frequency(request->pin(), request->frequency_hz());

    return ::grpc::Status();
}

::grpc::Status BoardServer::DoCommand(grpc::ServerContext* context,
                                      const viam::common::v1::DoCommandRequest* request,
                                      viam::common::v1::DoCommandResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Board::DoCommand] without a request");
    };

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);
    AttributeMap result = board->do_command(struct_to_map(request->command()));

    *response->mutable_result() = map_to_struct(result);

    return ::grpc::Status();
}

::grpc::Status BoardServer::ReadAnalogReader(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::ReadAnalogReaderRequest* request,
    ::viam::component::board::v1::ReadAnalogReaderResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Board::ReadAnalogReader] without a request");
    };

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    Board::analog_value result =
        board->read_analog(request->board_name(), request->analog_reader_name());
    response->set_value(result.value);

    return ::grpc::Status();
}

::grpc::Status BoardServer::GetDigitalInterruptValue(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::GetDigitalInterruptValueRequest* request,
    ::viam::component::board::v1::GetDigitalInterruptValueResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Board::GetDigitalInterruptValue] without a request");
    };

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    Board::digital_value result =
        board->read_digital_interrupt(request->board_name(), request->digital_interrupt_name());
    response->set_value(result.value);

    return ::grpc::Status();
}

::grpc::Status BoardServer::SetPowerMode(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::SetPowerModeRequest* request,
    ::viam::component::board::v1::SetPowerModeResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Board::SetPowerMode] without a request");
    };

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    board->set_power_mode(request->power_mode(), request->duration());

    return ::grpc::Status();
}

void BoardServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(board);
}

}  // namespace sdk
}  // namespace viam
