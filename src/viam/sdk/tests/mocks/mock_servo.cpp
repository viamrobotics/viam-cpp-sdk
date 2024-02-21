#include <viam/sdk/tests/mocks/mock_servo.hpp>

#include <stdexcept>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/servo/v1/servo.grpc.pb.h>
#include <viam/api/component/servo/v1/servo.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/servo/server.hpp>
#include <viam/sdk/components/servo/servo.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace servo {

using namespace viam::sdk;

void MockServo::move(uint32_t angle_deg, const AttributeMap&) {
    is_moving_ = true;
    position_ = angle_deg;
}

Servo::position MockServo::get_position(const AttributeMap&) {
    return position_;
}

void MockServo::stop(const AttributeMap&) {
    is_moving_ = false;
}

std::vector<GeometryConfig> MockServo::get_geometries(const sdk::AttributeMap&) {
    return fake_geometries();
}

bool MockServo::is_moving() {
    return is_moving_;
}

AttributeMap MockServo::do_command(const AttributeMap&) {
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
