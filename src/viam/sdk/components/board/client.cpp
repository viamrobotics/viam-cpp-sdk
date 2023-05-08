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

BoardClient::BoardClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Board(std::move(name)),
      stub_(viam::component::board::v1::BoardService::NewStub(channel)),
      channel_(std::move(channel)){};

Board::status BoardClient::get_status() {
    viam::component::board::v1::StatusRequest request;
    viam::component::board::v1::StatusResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();

    grpc::Status status = stub_->Status(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response.status());
}

void BoardClient::set_gpio(const std::string& pin, bool high) {
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

bool BoardClient::get_gpio(const std::string& pin) {
    viam::component::board::v1::GetGPIORequest request;
    viam::component::board::v1::GetGPIOResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_pin(pin);

    grpc::Status status = stub_->GetGPIO(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return response.high();
}

double BoardClient::get_pwm_duty_cycle(const std::string& pin) {
    viam::component::board::v1::PWMRequest request;
    viam::component::board::v1::PWMResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_pin(pin);

    grpc::Status status = stub_->PWM(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return response.duty_cycle_pct();
}

void BoardClient::set_pwm_duty_cycle(const std::string& pin, double duty_cycle_pct) {
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

uint64_t BoardClient::get_pwm_frequency(const std::string& pin) {
    viam::component::board::v1::PWMFrequencyRequest request;
    viam::component::board::v1::PWMFrequencyResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_pin(pin);

    grpc::Status status = stub_->PWMFrequency(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return response.frequency_hz();
}

void BoardClient::set_pwm_frequency(const std::string& pin, uint64_t frequency_hz) {
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

AttributeMap BoardClient::do_command(AttributeMap command) {
    viam::common::v1::DoCommandRequest request;
    viam::common::v1::DoCommandResponse response;

    grpc::ClientContext ctx;

    google::protobuf::Struct proto_command = map_to_struct(command);
    *request.mutable_command() = proto_command;
    *request.mutable_name() = this->name();

    grpc::Status status = stub_->DoCommand(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return struct_to_map(response.result());
}

Board::analog_value BoardClient::read_analog(const std::string& analog_reader_name) {
    viam::component::board::v1::ReadAnalogReaderRequest request;
    viam::component::board::v1::ReadAnalogReaderResponse response;

    grpc::ClientContext ctx;

    request.set_board_name(this->name());
    request.set_analog_reader_name(analog_reader_name);

    grpc::Status status = stub_->ReadAnalogReader(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return response.value();
}

Board::digital_value BoardClient::read_digital_interrupt(
    const std::string& digital_interrupt_name) {
    viam::component::board::v1::GetDigitalInterruptValueRequest request;
    viam::component::board::v1::GetDigitalInterruptValueResponse response;

    grpc::ClientContext ctx;

    request.set_board_name(this->name());
    request.set_digital_interrupt_name(digital_interrupt_name);

    grpc::Status status = stub_->GetDigitalInterruptValue(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return response.value();
}

void BoardClient::set_power_mode(
    power_mode power_mode,
    boost::optional<const std::chrono::duration<int64_t, std::micro>&> duration) {
    viam::component::board::v1::SetPowerModeRequest request;
    viam::component::board::v1::SetPowerModeResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_power_mode(to_proto(power_mode));
    if (duration.has_value()) {
        *request.mutable_duration() = ::viam::sdk::to_proto(duration.get());
    }

    grpc::Status status = stub_->SetPowerMode(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
}

std::vector<std::string> BoardClient::get_analog_reader_names() {
    std::vector<std::string> names;
    auto status = this->get_status();
    names.reserve(status.analog_reader_values.size());
    for (auto it = status.analog_reader_values.begin(); it != status.analog_reader_values.end();
         ++it) {
        names.push_back(it->first);
    }
    return names;
}

std::vector<std::string> BoardClient::get_digital_interrupt_names() {
    std::vector<std::string> names;
    auto status = this->get_status();
    names.reserve(status.digital_interrupt_values.size());
    for (auto it = status.digital_interrupt_values.begin();
         it != status.digital_interrupt_values.end();
         ++it) {
        names.push_back(it->first);
    }
    return names;
}

}  // namespace sdk
}  // namespace viam
