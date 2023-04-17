#define BOOST_TEST_MODULE test module test_motor

#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include <common/v1/common.pb.h>
#include <component/motor/v1/motor.grpc.pb.h>
#include <component/motor/v1/motor.pb.h>

#include <common/proto_type.hpp>
#include <components/motor/motor.hpp>
#include <components/motor/client.hpp>
#include <components/motor/server.hpp>
#include <tests/mocks/mock_motor.hpp>
#include <tests/test_utils.hpp>

namespace viam {
namespace cppsdktests {

using namespace motor;

using namespace viam::cppsdk;

BOOST_AUTO_TEST_SUITE(test_motor)


BOOST_AUTO_TEST_CASE(test_set_power) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_go_for) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_go_to) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_reset_zero_position) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_get_position) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_get_properties) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_stop_motor) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_get_power_status) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_is_moving) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockMotor> motor = MockMotor::get_mock_motor();
    // TODO impl
    BOOST_CHECK(false);
}


BOOST_AUTO_TEST_SUITE_END()

} // namespace cppsdktests
} // namespace viam
