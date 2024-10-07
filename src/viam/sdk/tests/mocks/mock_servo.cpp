#include <viam/sdk/tests/mocks/mock_servo.hpp>

#include <stdexcept>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/servo.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace servo {

using namespace viam::sdk;

void MockServo::move(uint32_t angle_deg, const ProtoStruct&) {
    is_moving_ = true;
    position_ = angle_deg;
}

Servo::position MockServo::get_position(const ProtoStruct&) {
    return position_;
}

void MockServo::stop(const ProtoStruct&) {
    is_moving_ = false;
}

std::vector<GeometryConfig> MockServo::get_geometries(const sdk::ProtoStruct&) {
    return fake_geometries();
}

bool MockServo::is_moving() {
    return is_moving_;
}

ProtoStruct MockServo::do_command(const ProtoStruct&) {
    return map_;
}

std::shared_ptr<MockServo> MockServo::get_mock_servo() {
    auto servo = std::make_shared<MockServo>("mock_servo");

    servo->position_ = fake_position();
    servo->map_ = fake_map();
    servo->is_moving_ = false;

    return servo;
}

Servo::position fake_position() {
    return 0.0;
}

}  // namespace servo
}  // namespace sdktests
}  // namespace viam
