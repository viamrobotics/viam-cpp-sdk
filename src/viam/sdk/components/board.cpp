#include <viam/sdk/components/board.hpp>

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

Board::status Board::from_proto(const viam::component::board::v1::Status& proto) {
    Board::status status;
    for (const auto& analog : proto.analogs()) {
        Board::analog_value result;
        result.value = analog.second;
        // The status does not contain the extra data to describe the accuracy of the reader. We
        // fill those in with 0's instead.
        result.min_range = 0.0;
        result.max_range = 0.0;
        result.step_size = 0.0;
        status.analog_reader_values.emplace(analog.first, result);
    }
    for (const auto& digital : proto.digital_interrupts()) {
        status.digital_interrupt_values.emplace(digital.first, digital.second);
    }
    return status;
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

viam::component::board::v1::Status Board::to_proto(const status& status) {
    viam::component::board::v1::Status proto;
    for (const auto& analog : status.analog_reader_values) {
        proto.mutable_analogs()->insert({analog.first, analog.second.value});
    }

    for (const auto& digital : status.digital_interrupt_values) {
        proto.mutable_digital_interrupts()->insert({digital.first, digital.second});
    }
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

Board::Board(std::string name) : Component(std::move(name)){};

bool operator==(const Board::status& lhs, const Board::status& rhs) {
    return (lhs.analog_reader_values == rhs.analog_reader_values &&
            lhs.digital_interrupt_values == rhs.digital_interrupt_values);
}

bool operator==(const Board::analog_value& lhs, const Board::analog_value& rhs) {
    return (lhs.value == rhs.value && lhs.min_range == rhs.min_range &&
            lhs.max_range == rhs.max_range && lhs.step_size == rhs.step_size);
}

}  // namespace sdk
}  // namespace viam
