#include <viam/sdk/components/board/board.hpp>

#include <google/protobuf/descriptor.h>

#include <viam/api/component/board/v1/board.grpc.pb.h>
#include <viam/api/component/board/v1/board.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/board/client.hpp>
#include <viam/sdk/components/board/server.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

std::shared_ptr<ResourceServerBase> BoardSubtype::create_resource_server(
    std::shared_ptr<SubtypeService> svc) {
    return std::make_shared<BoardServer>(svc);
};

std::shared_ptr<ResourceBase> BoardSubtype::create_rpc_client(
    std::string name, std::shared_ptr<grpc::Channel> chan) {
    return std::make_shared<BoardClient>(std::move(name), std::move(chan));
};

std::shared_ptr<ResourceSubtype> Board::resource_subtype() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::board::v1::BoardService::service_full_name());
    if (sd == nullptr) {
        throw std::runtime_error("Unable to get service descriptor for the board service");
    }
    return std::make_shared<BoardSubtype>(sd);
}

Subtype Board::subtype() {
    return Subtype(RDK, COMPONENT, "board");
}


Board::status Board::from_proto(viam::component::board::v1::StatusResponse proto){
    Board::status status;
    status.status = proto.status();
    return status;
}

Board::gpio_pin Board::from_proto(viam::component::board::v1::GetGPIOResponse proto){
    Board::gpio_pin gpio_pin;
    gpio_pin.high = proto.high();
    return gpio_pin;
}

Board::duty_cycle Board::from_proto(viam::component::board::v1::PWMResponse proto){
    Board::duty_cycle duty_cycle;
    duty_cycle.duty_cycle_pct = proto.duty_cycle_pct();
    return duty_cycle;
}

Board::pwm_frequency Board::from_proto(viam::component::board::v1::PWMFrequencyResponse proto){
    Board::pwm_frequency pwm_frequency;
    pwm_frequency.frequency_hz = proto.frequency_hz();
    return pwm_frequency;
}

Board::analog_value Board::from_proto(viam::component::board::v1::ReadAnalogReaderResponse proto){
    Board::analog_value analog_value;
    analog_value.value = proto.value();
    return analog_value;
}

Board::digital_value Board::from_proto(viam::component::board::v1::GetDigitalInterruptValueResponse proto){
    Board::digital_value digital_value;
    digital_value.value = proto.value();
    return digital_value;
}




viam::component::board::v1::StatusResponse Board::to_proto( status status) {
    viam::component::board::v1::StatusResponse proto;
    proto.set_status(status.status);
    return proto;
}


viam::component::board::v1::GetGPIOResponse Board::to_proto( gpio_pin gpio_pin) {
    viam::component::board::v1::GetGPIOResponse proto;
    proto.set_high(gpio_pin.high);
    return proto;
}


viam::component::board::v1::PWMResponse Board::to_proto( duty_cycle duty_cycle) {
    viam::component::board::v1::PWMResponse proto;
    proto.set_duty_cycle_pct(duty_cycle.duty_cycle_pct);
    return proto;
}


viam::component::board::v1::PWMFrequencyResponse Board::to_proto( pwm_frequency pwm_frequency) {
    viam::component::board::v1::PWMFrequencyResponse proto;
    proto.set_frequency_hz(pwm_frequency.frequency_hz);
    return proto;
}


viam::component::board::v1::ReadAnalogReaderResponse Board::to_proto( analog_value analog_value) {
    viam::component::board::v1::ReadAnalogReaderResponse proto;
    proto.set_value(analog_value.value);
    return proto;
}


viam::component::board::v1::GetDigitalInterruptValueResponse Board::to_proto( digital_value digital_value) {
    viam::component::board::v1::GetDigitalInterruptValueResponse proto;
    proto.set_value(digital_value.value);
    return proto;
}



bool operator==(const Board::status& lhs, const Board::status& rhs) {
    return (
        lhs.status == rhs.status);
}

bool operator==(const Board::gpio_pin& lhs, const Board::gpio_pin& rhs) {
    return (
        lhs.high == rhs.high);
}

bool operator==(const Board::duty_cycle& lhs, const Board::duty_cycle& rhs) {
    return (
        lhs.duty_cycle_pct == rhs.duty_cycle_pct);
}

bool operator==(const Board::pwm_frequency& lhs, const Board::pwm_frequency& rhs) {
    return (
        lhs.frequency_hz == rhs.frequency_hz);
}

bool operator==(const Board::analog_value& lhs, const Board::analog_value& rhs) {
    return (
        lhs.value == rhs.value);
}

bool operator==(const Board::digital_value& lhs, const Board::digital_value& rhs) {
    return (
        lhs.value == rhs.value);
}



namespace {
bool init() {
    Registry::register_subtype(Board::subtype(), Board::resource_subtype());
    return true;
};

bool inited = init();
}  // namespace

}  // namespace sdk
}  // namespace viam
