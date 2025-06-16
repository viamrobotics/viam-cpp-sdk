#include <viam/sdk/components/board.hpp>

#include <google/protobuf/descriptor.h>

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

Board::Board(std::string name) : Component(std::move(name)) {}

bool operator==(const Board::status& lhs, const Board::status& rhs) {
    return (lhs.analog_reader_values == rhs.analog_reader_values &&
            lhs.digital_interrupt_values == rhs.digital_interrupt_values);
}

}  // namespace sdk
}  // namespace viam
