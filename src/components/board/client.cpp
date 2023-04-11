#include <components/board/client.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include <common/v1/common.pb.h>
#include <component/board/v1/board.grpc.pb.h>

#include <common/utils.hpp>
#include <components/board/board.hpp>
#include <config/resource.hpp>
#include <robot/client.hpp>

AttributeMap BoardClient::do_command(AttributeMap command) {
    viam::common::v1::DoCommandRequest req;
    viam::common::v1::DoCommandResponse resp;
    grpc::ClientContext ctx;

    google::protobuf::Struct proto_command = map_to_struct(command);
    *req.mutable_command() = proto_command;
    *req.mutable_name() = this->name();
    stub_->DoCommand(&ctx, req, &resp);
    return struct_to_map(resp.result());
};

Board::board_status BoardClient::get_status() {
    viam::component::board::v1::StatusRequest req;
    viam::component::board::v1::StatusResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();

    stub_->Status(&ctx, req, &resp);
    return from_proto(resp.status());
};

void BoardClient::set_gpio(std::string pin, bool high) {
    viam::component::board::v1::SetGPIORequest req;
    viam::component::board::v1::SetGPIOResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();
    *req.mutable_pin() = pin;
    req.set_high(high);

    stub_->SetGPIO(&ctx, req, &resp);
};

bool BoardClient::get_gpio(std::string pin) {
    viam::component::board::v1::GetGPIORequest req;
    viam::component::board::v1::GetGPIOResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();
    *req.mutable_pin() = pin;

    stub_->GetGPIO(&ctx, req, &resp);
    return resp.high();
};

double BoardClient::get_pwm(std::string pin) {
    viam::component::board::v1::PWMRequest req;
    viam::component::board::v1::PWMResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();
    *req.mutable_pin() = pin;

    stub_->PWM(&ctx, req, &resp);
    return resp.duty_cycle_pct();
};

void BoardClient::set_pwm(std::string pin, double duty_cycle_pct) {
    viam::component::board::v1::SetPWMRequest req;
    viam::component::board::v1::SetPWMResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();
    *req.mutable_pin() = pin;
    req.set_duty_cycle_pct(duty_cycle_pct);

    stub_->SetPWM(&ctx, req, &resp);
};

uint64_t BoardClient::get_pwm_frequency(std::string pin) {
    viam::component::board::v1::PWMFrequencyRequest req;
    viam::component::board::v1::PWMFrequencyResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();
    *req.mutable_pin() = pin;

    stub_->PWMFrequency(&ctx, req, &resp);
    return resp.frequency_hz();
};

void BoardClient::set_pwm_frequency(std::string pin, uint64_t frequency_hz) {
    viam::component::board::v1::SetPWMFrequencyRequest req;
    viam::component::board::v1::SetPWMFrequencyResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();
    *req.mutable_pin() = pin;
    req.set_frequency_hz(frequency_hz);

    stub_->SetPWMFrequency(&ctx, req, &resp);
};

int32_t BoardClient::read_analog_reader(std::string analog_reader_name) {
    viam::component::board::v1::ReadAnalogReaderRequest req;
    viam::component::board::v1::ReadAnalogReaderResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_board_name() = this->name();
    *req.mutable_analog_reader_name() = analog_reader_name;

    stub_->ReadAnalogReader(&ctx, req, &resp);
    return resp.value();
};

int64_t BoardClient::get_digital_interrupt_value(std::string digital_interrupt_name) {
    viam::component::board::v1::GetDigitalInterruptValueRequest req;
    viam::component::board::v1::GetDigitalInterruptValueResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_board_name() = this->name();
    *req.mutable_digital_interrupt_name() = digital_interrupt_name;

    stub_->GetDigitalInterruptValue(&ctx, req, &resp);
    return resp.value();
};

void BoardClient::set_power_mode(Board::power_mode power_mode,
                                 const google::protobuf::Duration& duration) {
    viam::component::board::v1::SetPowerModeRequest req;
    viam::component::board::v1::SetPowerModeResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();
    req.set_power_mode(static_cast<viam::component::board::v1::PowerMode>(power_mode));
    *req.mutable_duration() = duration;

    stub_->SetPowerMode(&ctx, req, &resp);
};
