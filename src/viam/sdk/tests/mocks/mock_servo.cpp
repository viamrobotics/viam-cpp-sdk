#include <viam/sdk/tests/mocks/mock_servo.hpp>

#include <stdexcept>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/servo/v1/servo.grpc.pb.h>
#include <viam/api/component/servo/v1/servo.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/servo/servo.hpp>
#include <viam/sdk/components/servo/server.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace servo {

using namespace viam::sdk;

Servo::position MockServo::get_position(const AttributeMap& extra) {
    return position_;
}

void MockServo::stop(const AttributeMap& extra) {
    // None of these functions are async and this mock is not
    // thread-safe (Send, not Sync). The mock servo should never be
    // moving when this is called
}

std::vector<GeometryConfig> MockServo::get_geometries(const sdk::AttributeMap& extra) {
    return fake_geometries();
}

bool MockServo::is_moving() {
    // None of these functions are async and this mock is not
    // thread-safe (Send, not Sync)
    return false;
}

AttributeMap MockServo::do_command(const AttributeMap& _command) {
    return map_;
}

std::shared_ptr<MockServo> MockServo::get_mock_servo() {
    auto servo = std::make_shared<MockServo>("mock_servo");

    servo->position_ = fake_position();
    servo->map_ = fake_map();

    return servo;
}

Servo::position fake_position() {
    return 0.0;
}

}  // namespace servo
}  // namespace sdktests
}  // namespace viam
