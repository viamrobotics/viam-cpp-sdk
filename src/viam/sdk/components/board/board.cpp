#include <viam/sdk/components/board/board.hpp>

#include <google/protobuf/descriptor.h>

#include <viam/api/component/board/v1/board.grpc.pb.h>
#include <viam/api/component/board/v1/board.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

API Board::api() const {
    return API::get<Board>();
}

API API::traits<Board>::api() {
    return {kRDK, kComponent, "board"};
}

Board::status Board::from_proto(const viam::common::v1::BoardStatus& proto) {
    Board::status status;
    for (const auto& analog : proto.analogs()) {
        status.analog_reader_values.emplace(analog.first, analog.second.value());
    }
    for (const auto& digital : proto.digital_interrupts()) {
        status.digital_interrupt_values.emplace(digital.first, digital.second.value());
    }
    return status;
}

Board::analog_value Board::from_proto(const viam::common::v1::AnalogStatus& proto) {
    return proto.value();
}

Board::digital_value Board::from_proto(const viam::common::v1::DigitalInterruptStatus& proto) {
    return proto.value();
}

Board::power_mode Board::from_proto(viam::component::board::v1::PowerMode proto) {
    switch (proto) {
        case viam::component::board::v1::POWER_MODE_NORMAL: {
            return Board::power_mode::normal;
        }
        case viam::component::board::v1::POWER_MODE_OFFLINE_DEEP: {
            return Board::power_mode::offline_deep;
        }
        case viam::component::board::v1::POWER_MODE_UNSPECIFIED:
        default: {
            throw Exception(ErrorCondition::k_not_supported,
                            "Invalid proto board power_mode to decode");
        }
    }
}

viam::common::v1::BoardStatus Board::to_proto(const status& status) {
    viam::common::v1::BoardStatus proto;
    for (const auto& analog : status.analog_reader_values) {
        proto.mutable_analogs()->insert({analog.first, to_proto(analog.second)});
    }

    for (const auto& digital : status.digital_interrupt_values) {
        proto.mutable_digital_interrupts()->insert({digital.first, to_proto(digital.second)});
    }
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

viam::component::board::v1::PowerMode Board::to_proto(Board::power_mode power_mode) {
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

std::vector<std::string> Board::get_analog_reader_names() {
    std::vector<std::string> names;
    auto status = this->get_status();
    names.reserve(status.analog_reader_values.size());
    for (const auto& kv : status.analog_reader_values) {
        names.push_back(kv.first);
    }
    return names;
}

std::vector<std::string> Board::get_digital_interrupt_names() {
    std::vector<std::string> names;
    auto status = this->get_status();
    names.reserve(status.digital_interrupt_values.size());
    for (const auto& kv : status.digital_interrupt_values) {
        names.push_back(kv.first);
    }
    return names;
}

std::unordered_map<std::string, Board::analog_value> Board::get_analog_readers() {
    return this->get_status().analog_reader_values;
}

std::unordered_map<std::string, Board::digital_value> Board::get_digital_interrupts() {
    return this->get_status().digital_interrupt_values;
}
Board::Board(std::string name) : Component(std::move(name)){};

bool operator==(const Board::status& lhs, const Board::status& rhs) {
    return (lhs.analog_reader_values == rhs.analog_reader_values &&
            lhs.digital_interrupt_values == rhs.digital_interrupt_values);
}

}  // namespace sdk
}  // namespace viam
