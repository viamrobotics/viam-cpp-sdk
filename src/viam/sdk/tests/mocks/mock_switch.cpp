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

sdk::Switch::position_info MockSwitch::get_number_of_positions(const sdk::ProtoStruct&) {
    return position_info;
}

sdk::ProtoStruct MockSwitch::do_command(const sdk::ProtoStruct& command) {
    return (peek_command = command);
}

sdk::ProtoStruct MockSwitch::get_status() {
    return fake_status();
}

}  // namespace switch_
}  // namespace sdktests
}  // namespace viam
