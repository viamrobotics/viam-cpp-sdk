#include <viam/sdk/tests/mocks/mock_board.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/board/v1/board.grpc.pb.h>
#include <viam/api/component/board/v1/board.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/board/board.hpp>
#include <viam/sdk/components/board/server.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace board {

using namespace viam::sdk;

MockBoard::MockBoard(std::string name) : Board(std::move(name)){};

Board::status MockBoard::get_status(const AttributeMap& extra) {
    return this->peek_get_status_ret;
}

void MockBoard::set_gpio(const std::string& pin, bool high, const AttributeMap& extra) {
    this->peek_pin = pin;
    this->peek_set_gpio_high = high;
}

bool MockBoard::get_gpio(const std::string& pin, const AttributeMap& extra) {
    this->peek_pin = pin;
    return this->peek_get_gpio_ret;
}

double MockBoard::get_pwm_duty_cycle(const std::string& pin, const AttributeMap& extra) {
    this->peek_pin = pin;
    return this->peek_get_pwm_duty_cycle_ret;
}

void MockBoard::set_pwm_duty_cycle(const std::string& pin,
                                   double duty_cycle_pct,
                                   const AttributeMap& extra) {
    this->peek_pin = pin;
    this->peek_set_pwm_duty_cycle_pct = duty_cycle_pct;
}

uint64_t MockBoard::get_pwm_frequency(const std::string& pin, const AttributeMap& extra) {
    this->peek_pin = pin;
    return this->peek_get_pwm_frequency_ret;
}

void MockBoard::set_pwm_frequency(const std::string& pin,
                                  uint64_t frequency_hz,
                                  const AttributeMap& extra) {
    this->peek_pin = pin;
    this->peek_set_pwm_frequency_hz = frequency_hz;
}

AttributeMap MockBoard::do_command(const AttributeMap& command) {
    return command;
}

Board::analog_value MockBoard::read_analog(const std::string& analog_reader_name,
                                           const AttributeMap& extra) {
    this->peek_analog_reader_name = analog_reader_name;
    return this->peek_read_analog_ret;
}

Board::digital_value MockBoard::read_digital_interrupt(const std::string& digital_interrupt_name,
                                                       const AttributeMap& extra) {
    this->peek_digital_interrupt_name = digital_interrupt_name;
    return this->peek_read_digital_interrupt_ret;
}

void MockBoard::set_power_mode(power_mode power_mode,
                               const AttributeMap& extra,
                               const boost::optional<std::chrono::microseconds>& duration) {
    this->peek_set_power_mode_power_mode = power_mode;
    this->peek_set_power_mode_duration = duration;
}

std::vector<GeometryConfig> MockBoard::get_geometries(const sdk::AttributeMap& extra) {
    return fake_geometries();
}

}  // namespace board
}  // namespace sdktests
}  // namespace viam
