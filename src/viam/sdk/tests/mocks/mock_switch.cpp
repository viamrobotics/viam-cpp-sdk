#include <viam/sdk/tests/mocks/mock_switch.hpp>

#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace switch_ {

std::shared_ptr<MockSwitch> MockSwitch::get_mock_switch() {
    return std::make_shared<MockSwitch>("mock_switch");
}

void MockSwitch::set_position(uint32_t position, const sdk::ProtoStruct&) {
    current_position = position;
}

uint32_t MockSwitch::get_position(const sdk::ProtoStruct&) {
    return current_position;
}

uint32_t MockSwitch::get_number_of_positions(const sdk::ProtoStruct&) {
    return number_of_positions;
}

sdk::ProtoStruct MockSwitch::do_command(const sdk::ProtoStruct& command) {
    return (peek_command = command);
}

}  // namespace switch_
}  // namespace sdktests
}  // namespace viam
