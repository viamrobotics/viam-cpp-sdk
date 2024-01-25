#define BOOST_TEST_MODULE test module test_board

#include <algorithm>
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

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)

namespace viam {
namespace sdktests {

using namespace board;

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_board)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockBoard board("mock_board");
    auto api = board.api();
    auto static_api = API::get<Board>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "board");
}

BOOST_AUTO_TEST_CASE(test_status) {
    const auto mock = std::make_shared<MockBoard>("mock_board");
    client_to_mock_pipeline<BoardClient>(mock, [&](Board& client) {
        std::unordered_map<std::string, Board::analog_value> analogs;
        analogs.emplace("analog", 1);
        std::unordered_map<std::string, Board::digital_value> digitals;
        digitals.emplace("digital", 2);
        mock->peek_get_status_ret = Board::status{analogs, digitals};

        BOOST_CHECK(client.get_status() == mock->peek_get_status_ret);
    });
}

BOOST_AUTO_TEST_CASE(test_set_gpio) {
    const auto mock = std::make_shared<MockBoard>("mock_board");
    client_to_mock_pipeline<BoardClient>(mock, [&](Board& client) {
        client.set_gpio("t1", true);
        BOOST_CHECK_EQUAL("t1", mock->peek_pin);
        BOOST_CHECK(mock->peek_set_gpio_high);
        client.set_gpio("t2", false);
        BOOST_CHECK_EQUAL("t2", mock->peek_pin);
        BOOST_CHECK(!mock->peek_set_gpio_high);
    });
}

BOOST_AUTO_TEST_CASE(test_get_gpio) {
    const auto mock = std::make_shared<MockBoard>("mock_board");
    client_to_mock_pipeline<BoardClient>(mock, [&](Board& client) {
        mock->peek_get_gpio_ret = true;
        BOOST_CHECK(client.get_gpio("t1"));
        BOOST_CHECK_EQUAL("t1", mock->peek_pin);
    });
}

BOOST_AUTO_TEST_CASE(test_get_pwm_duty_cycle) {
    const auto mock = std::make_shared<MockBoard>("mock_board");
    client_to_mock_pipeline<BoardClient>(mock, [&](Board& client) {
        mock->peek_get_pwm_duty_cycle_ret = 55;
        BOOST_CHECK_EQUAL(55, client.get_pwm_duty_cycle("t1"));
        BOOST_CHECK_EQUAL("t1", mock->peek_pin);
    });
}

BOOST_AUTO_TEST_CASE(test_set_pwm_duty_cycle) {
    const auto mock = std::make_shared<MockBoard>("mock_board");
    client_to_mock_pipeline<BoardClient>(mock, [&](Board& client) {
        client.set_pwm_duty_cycle("t1", 0.512);
        BOOST_CHECK_EQUAL("t1", mock->peek_pin);
        BOOST_CHECK_EQUAL(0.512, mock->peek_set_pwm_duty_cycle_pct);
    });
}

BOOST_AUTO_TEST_CASE(test_get_pwm_frequency) {
    const auto mock = std::make_shared<MockBoard>("mock_board");
    client_to_mock_pipeline<BoardClient>(mock, [&](Board& client) {
        mock->peek_get_pwm_frequency_ret = 510;
        BOOST_CHECK_EQUAL(510, client.get_pwm_frequency("t1"));
        BOOST_CHECK_EQUAL("t1", mock->peek_pin);
    });
}

BOOST_AUTO_TEST_CASE(test_set_pwm_frequency) {
    const auto mock = std::make_shared<MockBoard>("mock_board");
    client_to_mock_pipeline<BoardClient>(mock, [&](Board& client) {
        client.set_pwm_frequency("t1", 10);
        BOOST_CHECK_EQUAL("t1", mock->peek_pin);
        BOOST_CHECK_EQUAL(10, mock->peek_set_pwm_frequency_hz);
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    const auto mock = std::make_shared<MockBoard>("mock_board");
    client_to_mock_pipeline<BoardClient>(mock, [](Board& client) {
        AttributeMap expected = fake_map();

        AttributeMap command = fake_map();
        AttributeMap result_map = client.do_command(command);

        ProtoType expected_pt = *(expected->at(std::string("test")));
        ProtoType result_pt = *(result_map->at(std::string("test")));
        BOOST_CHECK(result_pt == expected_pt);
    });
}

BOOST_AUTO_TEST_CASE(test_read_analog) {
    const auto mock = std::make_shared<MockBoard>("mock_board");
    client_to_mock_pipeline<BoardClient>(mock, [&](Board& client) {
        mock->peek_read_analog_ret = 5150;
        BOOST_CHECK_EQUAL(5150, client.read_analog("t1"));
        BOOST_CHECK_EQUAL("t1", mock->peek_analog_reader_name);
    });
}

BOOST_AUTO_TEST_CASE(test_write_analog) {
    const auto mock = std::make_shared<MockBoard>("mock_board");
    client_to_mock_pipeline<BoardClient>(mock, [&](Board& client) {
        std::string pin = "pin1";
        int value = 42;
        client.write_analog(pin, value);
        BOOST_CHECK_EQUAL(pin, mock->peek_pin);
        BOOST_CHECK_EQUAL(value, mock->peek_pin_value);
    });
}

BOOST_AUTO_TEST_CASE(test_read_digital_interrupt) {
    const auto mock = std::make_shared<MockBoard>("mock_board");
    client_to_mock_pipeline<BoardClient>(mock, [&](Board& client) {
        mock->peek_read_digital_interrupt_ret = 515;
        BOOST_CHECK_EQUAL(515, client.read_digital_interrupt("t1"));
        BOOST_CHECK_EQUAL("t1", mock->peek_digital_interrupt_name);
    });
}

BOOST_AUTO_TEST_CASE(test_get_analog_reader_names) {
    const auto mock = std::make_shared<MockBoard>("mock_board");
    client_to_mock_pipeline<BoardClient>(mock, [&](Board& client) {
        std::unordered_map<std::string, Board::analog_value> analogs;
        analogs.emplace("analog1", 2);
        analogs.emplace("analog2", 2);
        mock->peek_get_status_ret = Board::status{analogs, {}};
        auto ret = client.get_analog_reader_names();
        std::sort(ret.begin(), ret.end());
        BOOST_CHECK_EQUAL(ret[0], "analog1");
        BOOST_CHECK_EQUAL(ret[1], "analog2");
    });
}

BOOST_AUTO_TEST_CASE(test_get_digital_interrupt_names) {
    const auto mock = std::make_shared<MockBoard>("mock_board");
    client_to_mock_pipeline<BoardClient>(mock, [&](Board& client) {
        std::unordered_map<std::string, Board::digital_value> digitals;
        digitals.emplace("digital1", 2);
        digitals.emplace("digital2", 2);
        mock->peek_get_status_ret = Board::status{{}, digitals};
        auto ret = client.get_digital_interrupt_names();
        std::sort(ret.begin(), ret.end());
        BOOST_CHECK_EQUAL(ret[0], "digital1");
        BOOST_CHECK_EQUAL(ret[1], "digital2");
    });
}

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    const auto mock = std::make_shared<MockBoard>("mock_board");
    client_to_mock_pipeline<BoardClient>(mock, [](Board& client) {
        const auto& geometries = client.get_geometries();
        BOOST_CHECK_EQUAL(geometries, fake_geometries());
    });
}

BOOST_AUTO_TEST_CASE(test_set_power_mode) {
    const auto mock = std::make_shared<MockBoard>("mock_board");
    client_to_mock_pipeline<BoardClient>(mock, [&](Board& client) {
        auto mode = Board::power_mode::normal;
        std::chrono::duration<int64_t, std::micro> duration = std::chrono::microseconds(15531000);
        client.set_power_mode(mode, duration);
        BOOST_CHECK(mode == mock->peek_set_power_mode_power_mode);
        BOOST_CHECK(duration == mock->peek_set_power_mode_duration.get());
        // Check with durations that have microsecond precision
        std::chrono::duration<int64_t, std::micro> duration_large =
            std::chrono::microseconds(3213001);
        client.set_power_mode(mode, duration_large);
        BOOST_CHECK(duration_large == mock->peek_set_power_mode_duration.get());

        client.set_power_mode(mode);
        BOOST_CHECK(!mock->peek_set_power_mode_duration.has_value());
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
