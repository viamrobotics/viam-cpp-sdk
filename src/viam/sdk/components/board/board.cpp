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
    std::shared_ptr<ResourceManager> manager) {
    return std::make_shared<BoardServer>(manager);
};

std::shared_ptr<ResourceBase> BoardSubtype::create_rpc_client(std::string name,
                                                              std::shared_ptr<grpc::Channel> chan) {
    return std::make_shared<BoardClient>(std::move(name), std::move(chan));
};

std::shared_ptr<ResourceSubtype> Board::resource_subtype() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::board::v1::BoardService::service_full_name());
    if (!sd) {
        throw std::runtime_error("Unable to get service descriptor for the board service");
    }
    return std::make_shared<BoardSubtype>(sd);
}

Subtype Board::subtype() {
    return Subtype(RDK, COMPONENT, "board");
}

Board::status Board::from_proto(viam::common::v1::BoardStatus proto) {
    Board::status status;
    for (const auto& analog : proto.analogs()) {
        status.analog_values.emplace(analog.first, analog.second.value());
    }
    for (const auto& digital : proto.digital_interrupts()) {
        status.digital_values.emplace(digital.first, digital.second.value());
    }
    return status;
}

Board::gpio_pin Board::from_proto(viam::component::board::v1::GetGPIOResponse proto) {
    Board::gpio_pin gpio_pin;
    gpio_pin.high = proto.high();
    return gpio_pin;
}

Board::duty_cycle Board::from_proto(viam::component::board::v1::PWMResponse proto) {
    Board::duty_cycle duty_cycle;
    duty_cycle.duty_cycle_pct = proto.duty_cycle_pct();
    return duty_cycle;
}

Board::analog_value Board::from_proto(viam::common::v1::AnalogStatus proto) {
    return proto.value();
}

Board::digital_value Board::from_proto(viam::common::v1::DigitalInterruptStatus proto) {
    return proto.value();
}

Board::power_mode Board::from_proto(viam::component::board::v1::PowerMode proto) {
    switch (proto) {
        case viam::component::board::v1::POWER_MODE_NORMAL: {
            return Board::power_mode::POWER_MODE_NORMAL;
        }
        case viam::component::board::v1::POWER_MODE_UNSPECIFIED: {
            return Board::power_mode::POWER_MODE_UNSPECIFIED;
        }
        case viam::component::board::v1::POWER_MODE_OFFLINE_DEEP: {
            return Board::power_mode::POWER_MODE_OFFLINE_DEEP;
        }
        default: {
            throw std::runtime_error("Invalid proto board power_mode to decode");
        }
    }
}
std::chrono::duration<double> Board::from_proto(google::protobuf::Duration proto) {
    /* std::chrono::duration<double, std::chrono::seconds> value(0); */
    auto value = std::chrono::seconds(10);
    /* value += std::chrono::duration<double, std::chrono::nanoseconds>((double)proto.nanos()); */
    return value;
}

viam::common::v1::BoardStatus Board::to_proto(status status) {
    viam::common::v1::BoardStatus proto;
    for (const auto& analog : status.analog_values) {
        proto.mutable_analogs()->insert({analog.first, to_proto(analog.second)});
    }

    for (const auto& digital : status.digital_values) {
        proto.mutable_digital_interrupts()->insert({digital.first, to_proto(digital.second)});
    }
    return proto;
}

viam::component::board::v1::GetGPIOResponse Board::to_proto(gpio_pin gpio_pin) {
    viam::component::board::v1::GetGPIOResponse proto;
    proto.set_high(gpio_pin.high);
    return proto;
}

viam::component::board::v1::PWMResponse Board::to_proto(duty_cycle duty_cycle) {
    viam::component::board::v1::PWMResponse proto;
    proto.set_duty_cycle_pct(duty_cycle.duty_cycle_pct);
    return proto;
}

viam::common::v1::AnalogStatus Board::to_proto(analog_value analog_value) {
    viam::common::v1::AnalogStatus proto;
    proto.set_value(analog_value);
    return proto;
}

viam::common::v1::DigitalInterruptStatus Board::to_proto(digital_value digital_value) {
    viam::common::v1::DigitalInterruptStatus proto;
    proto.set_value(digital_value);
    return proto;
}

viam::component::board::v1::PowerMode Board::to_proto(Board::power_mode proto) {
    switch (proto) {
        case Board::power_mode::POWER_MODE_NORMAL: {
            return viam::component::board::v1::POWER_MODE_NORMAL;
        }
        case Board::power_mode::POWER_MODE_OFFLINE_DEEP: {
            return viam::component::board::v1::POWER_MODE_OFFLINE_DEEP;
        }
        case Board::power_mode::POWER_MODE_UNSPECIFIED: {
            return viam::component::board::v1::POWER_MODE_UNSPECIFIED;
        }
        default: {
            throw std::runtime_error("Invalid board power_mode to encode");
        }
    }
}

google::protobuf::Duration Board::to_proto(std::chrono::duration<double> duration) {
    google::protobuf::Duration proto;
    // TODO
    proto.set_nanos(0);
    proto.set_seconds(0);
    return proto;
}

bool operator==(const Board::status& lhs, const Board::status& rhs) {
    return (lhs.analog_values == rhs.analog_values && lhs.digital_values == rhs.digital_values);
}

bool operator==(const Board::gpio_pin& lhs, const Board::gpio_pin& rhs) {
    return (lhs.high == rhs.high);
}

bool operator==(const Board::duty_cycle& lhs, const Board::duty_cycle& rhs) {
    return (lhs.duty_cycle_pct == rhs.duty_cycle_pct);
}

bool operator==(const Board::pwm_frequency& lhs, const Board::pwm_frequency& rhs) {
    return (lhs.frequency_hz == rhs.frequency_hz);
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
