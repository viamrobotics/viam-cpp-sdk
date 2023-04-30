#include <viam/sdk/components/board/client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/board/v1/board.grpc.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/board/board.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {


Board::status BoardClient::status() {
    viam::component::board::v1::StatusRequest request;
    viam::component::board::v1::StatusResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();

    grpc::Status status = stub_->Status(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
    
}

void BoardClient::set_gpio(std::string pin, bool high) {
    viam::component::board::v1::SetGPIORequest request;
    viam::component::board::v1::SetGPIOResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_pin(pin);
    request.set_high(high);

    grpc::Status status = stub_->SetGPIO(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
}

Board::gpio_pin BoardClient::get_gpio(std::string pin) {
    viam::component::board::v1::GetGPIORequest request;
    viam::component::board::v1::GetGPIOResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_pin(pin);

    grpc::Status status = stub_->GetGPIO(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
    
}

Board::duty_cycle BoardClient::get_pwm(std::string pin) {
    viam::component::board::v1::PWMRequest request;
    viam::component::board::v1::PWMResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_pin(pin);

    grpc::Status status = stub_->PWM(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
    
}

void BoardClient::set_pwm(std::string pin, double duty_cycle_pct) {
    viam::component::board::v1::SetPWMRequest request;
    viam::component::board::v1::SetPWMResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_pin(pin);
    request.set_duty_cycle_pct(duty_cycle_pct);

    grpc::Status status = stub_->SetPWM(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
}

Board::pwm_frequency BoardClient::get_pwm_frequency(std::string pin) {
    viam::component::board::v1::PWMFrequencyRequest request;
    viam::component::board::v1::PWMFrequencyResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_pin(pin);

    grpc::Status status = stub_->PWMFrequency(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
    
}

void BoardClient::set_pwm_frequency(std::string pin, uint64_t frequency_hz) {
    viam::component::board::v1::SetPWMFrequencyRequest request;
    viam::component::board::v1::SetPWMFrequencyResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_pin(pin);
    request.set_frequency_hz(frequency_hz);

    grpc::Status status = stub_->SetPWMFrequency(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
}

AttributeMap BoardClient::do_command(ERROR TODO) {
    viam::component::board::v1::common.v1.DoCommandRequest request;
    viam::component::board::v1::common.v1.DoCommandResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_TODO(TODO);

    grpc::Status status = stub_->DoCommand(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
    
}

Board::analog_value BoardClient::read_analog(std::string board_name, std::string analog_reader_name) {
    viam::component::board::v1::ReadAnalogReaderRequest request;
    viam::component::board::v1::ReadAnalogReaderResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_board_name(board_name);
    request.set_analog_reader_name(analog_reader_name);

    grpc::Status status = stub_->ReadAnalogReader(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
    
}

Board::digital_value BoardClient::read_digital_interrupt(std::string board_name, std::string digital_interrupt_name) {
    viam::component::board::v1::GetDigitalInterruptValueRequest request;
    viam::component::board::v1::GetDigitalInterruptValueResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_board_name(board_name);
    request.set_digital_interrupt_name(digital_interrupt_name);

    grpc::Status status = stub_->GetDigitalInterruptValue(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
    
}

void BoardClient::set_power_mode(power_mode power_mode, google.protobuf.Duration duration) {
    viam::component::board::v1::SetPowerModeRequest request;
    viam::component::board::v1::SetPowerModeResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_power_mode(power_mode);
    request.set_duration(duration);

    grpc::Status status = stub_->SetPowerMode(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
}


}  // namespace sdk
}  // namespace viam
