#define BOOST_TEST_MODULE test module test_board

#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/board/v1/board.grpc.pb.h>
#include <viam/api/component/board/v1/board.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/board/board.hpp>
#include <viam/sdk/components/board/client.hpp>
#include <viam/sdk/components/board/server.hpp>
#include <viam/sdk/tests/mocks/mock_board.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {

using namespace board;

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_board)


BOOST_AUTO_TEST_CASE(test_status) {
    std::shared_ptr<MockBoard> board = MockBoard::get_mock_board();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_set_gpio) {
    std::shared_ptr<MockBoard> board = MockBoard::get_mock_board();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_get_gpio) {
    std::shared_ptr<MockBoard> board = MockBoard::get_mock_board();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_get_pwm) {
    std::shared_ptr<MockBoard> board = MockBoard::get_mock_board();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_set_pwm) {
    std::shared_ptr<MockBoard> board = MockBoard::get_mock_board();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_get_pwm_frequency) {
    std::shared_ptr<MockBoard> board = MockBoard::get_mock_board();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_set_pwm_frequency) {
    std::shared_ptr<MockBoard> board = MockBoard::get_mock_board();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockBoard> board = MockBoard::get_mock_board();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_read_analog) {
    std::shared_ptr<MockBoard> board = MockBoard::get_mock_board();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_read_digital_interrupt) {
    std::shared_ptr<MockBoard> board = MockBoard::get_mock_board();
    // TODO impl
    BOOST_CHECK(false);
}

BOOST_AUTO_TEST_CASE(test_set_power_mode) {
    std::shared_ptr<MockBoard> board = MockBoard::get_mock_board();
    // TODO impl
    BOOST_CHECK(false);
}


BOOST_AUTO_TEST_SUITE_END()

} // namespace sdktests
} // namespace viam
