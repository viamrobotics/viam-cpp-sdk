#include <viam/sdk/tests/mocks/mock_board.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/board/v1/board.grpc.pb.h>
#include <viam/api/component/board/v1/board.pb.h>

#include <viam/sdk/components/board/board.hpp>
#include <viam/sdk/components/board/server.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace board {

using namespace viam::sdk;

Board::status MockBoard::status() {
    // TODO impl
    return; 
};
void MockBoard::set_gpio(std::string pin, bool high) {
    // TODO impl
    return; 
};
Board::gpio_pin MockBoard::get_gpio(std::string pin) {
    // TODO impl
    return; 
};
Board::duty_cycle MockBoard::get_pwm(std::string pin) {
    // TODO impl
    return; 
};
void MockBoard::set_pwm(std::string pin, double duty_cycle_pct) {
    // TODO impl
    return; 
};
Board::pwm_frequency MockBoard::get_pwm_frequency(std::string pin) {
    // TODO impl
    return; 
};
void MockBoard::set_pwm_frequency(std::string pin, uint64_t frequency_hz) {
    // TODO impl
    return; 
};
AttributeMap MockBoard::do_command(ERROR TODO) {
    // TODO impl
    return; 
};
Board::analog_value MockBoard::read_analog(std::string board_name, std::string analog_reader_name) {
    // TODO impl
    return; 
};
Board::digital_value MockBoard::read_digital_interrupt(std::string board_name, std::string digital_interrupt_name) {
    // TODO impl
    return; 
};
void MockBoard::set_power_mode(power_mode power_mode, google.protobuf.Duration duration) {
    // TODO impl
    return; 
};

std::shared_ptr<MockBoard> MockBoard::get_mock_board() {
    auto board = std::make_shared<MockBoard>("mock_board");
    // TODO board->attr = fake_attr();
    // auto-generated suggestions:
    board_->status = fake_status();
    board_->gpio_pin = fake_gpio_pin();
    board_->duty_cycle = fake_duty_cycle();
    board_->pwm_frequency = fake_pwm_frequency();
    board_->analog_value = fake_analog_value();
    board_->digital_value = fake_digital_value();

    return board;
}

// TODO auto-generated suggestions:
Board::status fake_status() {
    Board::status status;
    status.status = TODO;
    return status;
}

Board::gpio_pin fake_gpio_pin() {
    Board::gpio_pin gpio_pin;
    gpio_pin.high = TODO;
    return gpio_pin;
}

Board::duty_cycle fake_duty_cycle() {
    Board::duty_cycle duty_cycle;
    duty_cycle.duty_cycle_pct = TODO;
    return duty_cycle;
}

Board::pwm_frequency fake_pwm_frequency() {
    Board::pwm_frequency pwm_frequency;
    pwm_frequency.frequency_hz = TODO;
    return pwm_frequency;
}

Board::analog_value fake_analog_value() {
    Board::analog_value analog_value;
    analog_value.value = TODO;
    return analog_value;
}

Board::digital_value fake_digital_value() {
    Board::digital_value digital_value;
    digital_value.value = TODO;
    return digital_value;
}


}  // namespace board
}  // namespace sdktests
}  // namespace viam
