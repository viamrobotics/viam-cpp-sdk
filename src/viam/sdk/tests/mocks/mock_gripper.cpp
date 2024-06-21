#include <viam/sdk/tests/mocks/mock_gripper.hpp>

#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace gripper {

using namespace viam::sdk;

std::shared_ptr<MockGripper> MockGripper::get_mock_gripper() {
    return std::make_shared<MockGripper>("mock_gripper");
}

void MockGripper::open(const AttributeMap&) {
    peek_open_called = true;
}

bool MockGripper::grab(const AttributeMap&) {
    return true;
}

void MockGripper::stop(const AttributeMap&) {
    peek_stop_called = true;
}

bool MockGripper::is_moving() {
    return false;
}

AttributeMap MockGripper::do_command(const AttributeMap& command) {
    return (peek_command = command);
}

std::vector<GeometryConfig> MockGripper::get_geometries(const AttributeMap&) {
    return fake_geometries();
}

}  // namespace gripper
}  // namespace sdktests
}  // namespace viam
