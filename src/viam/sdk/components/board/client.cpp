#include <viam/sdk/components/board/client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/board/v1/board.grpc.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/board/board.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

BoardClient::BoardClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Board(std::move(name)),
      stub_(viam::component::board::v1::BoardService::NewStub(channel)),
      channel_(std::move(channel)){};

Board::status BoardClient::get_status(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::Status)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response.status()); });
}

void BoardClient::set_gpio(const std::string& pin, bool high, const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::SetGPIO)
        .with(extra,
              [&](auto& request) {
                  request.set_pin(pin);
                  request.set_high(high);
              })
        .invoke();
}

bool BoardClient::get_gpio(const std::string& pin, const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGPIO)
        .with(extra, [&](auto& request) { request.set_pin(pin); })
        .invoke([](auto& response) { return response.high(); });
}

double BoardClient::get_pwm_duty_cycle(const std::string& pin, const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::PWM)
        .with(extra, [&](auto& request) { request.set_pin(pin); })
        .invoke([](auto& response) { return response.duty_cycle_pct(); });
}

void BoardClient::set_pwm_duty_cycle(const std::string& pin,
                                     double duty_cycle_pct,
                                     const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::SetPWM)
        .with(extra,
              [&](auto& request) {
                  request.set_pin(pin);
                  request.set_duty_cycle_pct(duty_cycle_pct);
              })
        .invoke();
}

uint64_t BoardClient::get_pwm_frequency(const std::string& pin, const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::PWMFrequency)
        .with(extra, [&](auto& request) { request.set_pin(pin); })
        .invoke([](auto& response) { return response.frequency_hz(); });
}

void BoardClient::set_pwm_frequency(const std::string& pin,
                                    uint64_t frequency_hz,
                                    const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::SetPWMFrequency)
        .with(extra,
              [&](auto& request) {
                  request.set_pin(pin);
                  request.set_frequency_hz(frequency_hz);
              })
        .invoke();
}

AttributeMap BoardClient::do_command(const AttributeMap& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
}

// TODO(RSDK-6048) update `client_wrapper` to allow for requests without a `mutable_name()` method,
// then wrap here.
Board::analog_value BoardClient::read_analog(const std::string& analog_reader_name,
                                             const AttributeMap& extra) {
    viam::component::board::v1::ReadAnalogReaderRequest request;
    viam::component::board::v1::ReadAnalogReaderResponse response;
    ClientContext ctx;

    request.set_board_name(this->name());
    request.set_analog_reader_name(analog_reader_name);
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->ReadAnalogReader(ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return response.value();
}

void BoardClient::write_analog(const std::string& pin, int value, const AttributeMap& extra) {
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
                                                         const AttributeMap& extra) {
    viam::component::board::v1::GetDigitalInterruptValueRequest request;
    viam::component::board::v1::GetDigitalInterruptValueResponse response;
    ClientContext ctx;

    request.set_board_name(this->name());
    request.set_digital_interrupt_name(digital_interrupt_name);
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->GetDigitalInterruptValue(ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return response.value();
}

void BoardClient::set_power_mode(power_mode power_mode,
                                 const AttributeMap& extra,
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

std::vector<GeometryConfig> BoardClient::get_geometries(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return GeometryConfig::from_proto(response); });
};

}  // namespace sdk
}  // namespace viam
