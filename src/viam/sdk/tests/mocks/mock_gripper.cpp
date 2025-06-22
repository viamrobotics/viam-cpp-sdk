#include <viam/sdk/tests/mocks/mock_gripper.hpp>

#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace gripper {

using namespace viam::sdk;

std::shared_ptr<MockGripper> MockGripper::get_mock_gripper() {
    return std::make_shared<MockGripper>("mock_gripper");
}

void MockGripper::open(const ProtoStruct&) {
    peek_open_called = true;
}

bool MockGripper::grab(const ProtoStruct&) {
    return true;
}

void MockGripper::stop(const ProtoStruct&) {
    peek_stop_called = true;
}

bool MockGripper::is_moving() {
    return false;
}

Gripper::holding_status MockGripper::is_holding_something(const ProtoStruct& extra) {
    Gripper::holding_status res;
    res.is_holding_something = false;
    res.meta = extra;
    return res;
}

ProtoStruct MockGripper::do_command(const ProtoStruct& command) {
    return (peek_command = command);
}

std::vector<GeometryConfig> MockGripper::get_geometries(const ProtoStruct&) {
    return fake_geometries();
}

}  // namespace gripper
}  // namespace sdktests
}  // namespace viam
