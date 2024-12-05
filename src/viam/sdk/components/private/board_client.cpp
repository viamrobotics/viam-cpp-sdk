#include <viam/sdk/components/private/board_client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/board/v1/board.grpc.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/board.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

viam::component::board::v1::Status to_proto(const Board::status& status) {
    viam::component::board::v1::Status proto;
    for (const auto& analog : status.analog_reader_values) {
        proto.mutable_analogs()->insert({analog.first, analog.second});
    }

    for (const auto& digital : status.digital_interrupt_values) {
        proto.mutable_digital_interrupts()->insert({digital.first, digital.second});
    }
    return proto;
}

viam::component::board::v1::PowerMode to_proto(Board::power_mode power_mode) {
    switch (power_mode) {
        case Board::power_mode::normal: {
            return viam::component::board::v1::POWER_MODE_NORMAL;
        }
        case Board::power_mode::offline_deep: {
            return viam::component::board::v1::POWER_MODE_OFFLINE_DEEP;
        }
        default: {
            throw Exception(ErrorCondition::k_not_supported, "Invalid board power_mode to encode");
        }
    }
}

BoardClient::BoardClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Board(std::move(name)),
      stub_(viam::component::board::v1::BoardService::NewStub(channel)),
      channel_(std::move(channel)) {}

void BoardClient::set_gpio(const std::string& pin, bool high, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::SetGPIO)
        .with(extra,
              [&](auto& request) {
                  request.set_pin(pin);
                  request.set_high(high);
              })
        .invoke();
}

bool BoardClient::get_gpio(const std::string& pin, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGPIO)
        .with(extra, [&](auto& request) { request.set_pin(pin); })
        .invoke([](auto& response) { return response.high(); });
}

double BoardClient::get_pwm_duty_cycle(const std::string& pin, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::PWM)
        .with(extra, [&](auto& request) { request.set_pin(pin); })
        .invoke([](auto& response) { return response.duty_cycle_pct(); });
}

void BoardClient::set_pwm_duty_cycle(const std::string& pin,
                                     double duty_cycle_pct,
                                     const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::SetPWM)
        .with(extra,
              [&](auto& request) {
                  request.set_pin(pin);
                  request.set_duty_cycle_pct(duty_cycle_pct);
              })
        .invoke();
}

uint64_t BoardClient::get_pwm_frequency(const std::string& pin, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::PWMFrequency)
        .with(extra, [&](auto& request) { request.set_pin(pin); })
        .invoke([](auto& response) { return response.frequency_hz(); });
}

void BoardClient::set_pwm_frequency(const std::string& pin,
                                    uint64_t frequency_hz,
                                    const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::SetPWMFrequency)
        .with(extra,
              [&](auto& request) {
                  request.set_pin(pin);
                  request.set_frequency_hz(frequency_hz);
              })
        .invoke();
}

ProtoStruct BoardClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = v2::to_proto(command); })
        .invoke([](auto& response) { return v2::from_proto(response.result()); });
}

// TODO(RSDK-6048) update `client_wrapper` to allow for requests without a `mutable_name()` method,
// then wrap here.
Board::analog_response BoardClient::read_analog(const std::string& analog_reader_name,
                                                const ProtoStruct& extra) {
    viam::component::board::v1::ReadAnalogReaderRequest request;
    viam::component::board::v1::ReadAnalogReaderResponse response;
    ClientContext ctx;

    request.set_board_name(this->name());
    request.set_analog_reader_name(analog_reader_name);
    *request.mutable_extra() = v2::to_proto(extra);

    const grpc::Status status = stub_->ReadAnalogReader(ctx, request, &response);
    if (!status.ok()) {
        throw GRPCException(status);
    }
    return Board::analog_response{
        response.value(), response.min_range(), response.max_range(), response.step_size()};
}

void BoardClient::write_analog(const std::string& pin, int value, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::WriteAnalog)
        .with(extra,
              [&](auto& request) {
                  request.set_pin(pin);
                  request.set_value(value);
              })
        .invoke();
}

// TODO(RSDK-6048) update `client_wrapper` to allow for requests without a `mutable_name()` method,
// then wrap here.
Board::digital_value BoardClient::read_digital_interrupt(const std::string& digital_interrupt_name,
                                                         const ProtoStruct& extra) {
    viam::component::board::v1::GetDigitalInterruptValueRequest request;
    viam::component::board::v1::GetDigitalInterruptValueResponse response;
    ClientContext ctx;

    request.set_board_name(this->name());
    request.set_digital_interrupt_name(digital_interrupt_name);
    *request.mutable_extra() = v2::to_proto(extra);

    const grpc::Status status = stub_->GetDigitalInterruptValue(ctx, request, &response);
    if (!status.ok()) {
        throw GRPCException(status);
    }
    return response.value();
}

void BoardClient::stream_ticks(std::vector<std::string> const& digital_interrupt_names,
                               std::function<bool(Tick&& tick)> const& tick_handler,
                               const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::StreamTicks)
        .with(extra,
              [&](auto& request) {
                  for (const auto& name : digital_interrupt_names) {
                      request.add_pin_names(name);
                  }
              })
        .invoke_stream([&](auto& response) {
            return tick_handler(
                {response.pin_name(), std::chrono::nanoseconds(response.time()), response.high()});
        });
}

void BoardClient::set_power_mode(power_mode power_mode,
                                 const ProtoStruct& extra,
                                 const boost::optional<std::chrono::microseconds>& duration) {
    return make_client_helper(this, *stub_, &StubType::SetPowerMode)
        .with(extra,
              [&](auto& request) {
                  request.set_power_mode(to_proto(power_mode));
                  if (duration.has_value()) {
                      *request.mutable_duration() = ::viam::sdk::to_proto(duration.get());
                  }
              })
        .invoke();
}

std::vector<GeometryConfig> BoardClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return v2::from_proto(response); });
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
