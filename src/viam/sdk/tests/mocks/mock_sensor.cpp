#include <viam/sdk/tests/mocks/mock_sensor.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/sensor/v1/sensor.grpc.pb.h>
#include <viam/api/component/sensor/v1/sensor.pb.h>

#include <viam/sdk/components/sensor/sensor.hpp>
#include <viam/sdk/components/sensor/server.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace sensor {

using namespace viam::sdk;

AttributeMap MockSensor::get_readings(const AttributeMap& extra) {
    return fake_map();
};
AttributeMap MockSensor::do_command(const AttributeMap& command) {
    return command;
};
std::vector<sdk::GeometryConfig> MockSensor::get_geometries(const AttributeMap& extra) {
    return fake_geometries();
};

std::shared_ptr<MockSensor> MockSensor::get_mock_sensor() {
    auto sensor = std::make_shared<MockSensor>("mock_sensor");
    return sensor;
}

}  // namespace sensor
}  // namespace sdktests
}  // namespace viam
