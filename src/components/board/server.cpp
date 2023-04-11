#include <components/board/server.hpp>

#include <common/utils.hpp>
#include <components/board/board.hpp>
#include <config/resource.hpp>
#include <rpc/server.hpp>

::grpc::Status BoardServer::DoCommand(::grpc::ServerContext* context,
                                      const ::viam::common::v1::DoCommandRequest* request,
                                      ::viam::common::v1::DoCommandResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [DoCommand] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);
    AttributeMap result = board->do_command(struct_to_map(request->command()));

    *response->mutable_result() = map_to_struct(result);

    return ::grpc::Status();
}

::grpc::Status BoardServer::Status(::grpc::ServerContext* context,
                                   const ::viam::component::board::v1::StatusRequest* request,
                                   ::viam::component::board::v1::StatusResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Status] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    viam::common::v1::BoardStatus board_status = Board::to_proto(board->get_status());

    *response->mutable_status() = board_status;

    return ::grpc::Status();
}

::grpc::Status BoardServer::SetGPIO(::grpc::ServerContext* context,
                                    const ::viam::component::board::v1::SetGPIORequest* request,
                                    ::viam::component::board::v1::SetGPIOResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [SetGPIO] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    board->set_gpio(request->pin(), request->high());

    return ::grpc::Status();
}

::grpc::Status BoardServer::GetGPIO(::grpc::ServerContext* context,
                                    const ::viam::component::board::v1::GetGPIORequest* request,
                                    ::viam::component::board::v1::GetGPIOResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [GetGPIO] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    bool gpio_value = board->get_gpio(request->pin());

    response->set_high(gpio_value);

    return ::grpc::Status();
}

::grpc::Status BoardServer::PWM(::grpc::ServerContext* context,
                                const ::viam::component::board::v1::PWMRequest* request,
                                ::viam::component::board::v1::PWMResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [PWM] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    double duty_cycle_pct = board->get_pwm(request->pin());

    response->set_duty_cycle_pct(duty_cycle_pct);

    return ::grpc::Status();
}

::grpc::Status BoardServer::SetPWM(::grpc::ServerContext* context,
                                   const ::viam::component::board::v1::SetPWMRequest* request,
                                   ::viam::component::board::v1::SetPWMResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [SetPWM] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
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
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [PWMFrequency] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    uint64_t frequency_hz = board->get_pwm_frequency(request->pin());

    response->set_frequency_hz(frequency_hz);

    return ::grpc::Status();
}

::grpc::Status BoardServer::SetPWMFrequency(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::SetPWMFrequencyRequest* request,
    ::viam::component::board::v1::SetPWMFrequencyResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [SetPWMFrequency] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    board->set_pwm_frequency(request->pin(), request->frequency_hz());

    return ::grpc::Status();
}

::grpc::Status BoardServer::ReadAnalogReader(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::ReadAnalogReaderRequest* request,
    ::viam::component::board::v1::ReadAnalogReaderResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [ReadAnalogReader] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->board_name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->board_name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    int32_t value = board->read_analog_reader(request->analog_reader_name());

    response->set_value(value);

    return ::grpc::Status();
}

::grpc::Status BoardServer::GetDigitalInterruptValue(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::GetDigitalInterruptValueRequest* request,
    ::viam::component::board::v1::GetDigitalInterruptValueResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [GetDigitalInterruptValue] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->board_name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->board_name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    int64_t value = board->get_digital_interrupt_value(request->digital_interrupt_name());

    response->set_value(value);

    return ::grpc::Status();
}

::grpc::Status BoardServer::SetPowerMode(
    ::grpc::ServerContext* context,
    const ::viam::component::board::v1::SetPowerModeRequest* request,
    ::viam::component::board::v1::SetPowerModeResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [SetPowerMode] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Board> board = std::dynamic_pointer_cast<Board>(rb);

    board->set_power_mode(Board::from_proto(request->power_mode()), request->duration());

    return ::grpc::Status();
}

void BoardServer::register_server() {
    viam::component::board::v1::BoardService::Service* board =
        static_cast<viam::component::board::v1::BoardService::Service*>(this);
    Server::register_service(board);
}

std::shared_ptr<SubtypeService> BoardServer::get_sub_svc() {
    return sub_svc;
}
