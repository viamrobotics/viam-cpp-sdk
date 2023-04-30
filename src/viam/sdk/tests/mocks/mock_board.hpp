#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/board/v1/board.grpc.pb.h>
#include <viam/api/component/board/v1/board.pb.h>

#include <viam/sdk/components/board/board.hpp>
#include <viam/sdk/components/board/client.hpp>
#include <viam/sdk/components/board/server.hpp>

namespace viam {
namespace sdktests {
namespace board {

class MockBoard : public viam::sdk::Board {
    public:
    viam::sdk::Board::status status() override;
    void set_gpio(std::string pin, bool high) override;
    viam::sdk::Board::gpio_pin get_gpio(std::string pin) override;
    viam::sdk::Board::duty_cycle get_pwm(std::string pin) override;
    void set_pwm(std::string pin, double duty_cycle_pct) override;
    viam::sdk::Board::pwm_frequency get_pwm_frequency(std::string pin) override;
    void set_pwm_frequency(std::string pin, uint64_t frequency_hz) override;
    AttributeMap do_command(ERROR TODO) override;
    viam::sdk::Board::analog_value read_analog(std::string board_name, std::string analog_reader_name) override;
    viam::sdk::Board::digital_value read_digital_interrupt(std::string board_name, std::string digital_interrupt_name) override;
    void set_power_mode(power_mode power_mode, google.protobuf.Duration duration) override;
    static std::shared_ptr<MockBoard> get_mock_board();

    MockBoard(std::string name) : Board(std::move(name)){};

   private:
    // TODO add private members for mock
    // auto-generated suggestions:
    viam::sdk::Board::status status_;
    viam::sdk::Board::gpio_pin gpio_pin_;
    viam::sdk::Board::duty_cycle duty_cycle_;
    viam::sdk::Board::pwm_frequency pwm_frequency_;
    viam::sdk::Board::analog_value analog_value_;
    viam::sdk::Board::digital_value digital_value_;
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> map;
};

// TODO create fake_xxx functions
// auto-generated suggestions:
viam::sdk::Board::status fake_status();
viam::sdk::Board::gpio_pin fake_gpio_pin();
viam::sdk::Board::duty_cycle fake_duty_cycle();
viam::sdk::Board::pwm_frequency fake_pwm_frequency();
viam::sdk::Board::analog_value fake_analog_value();
viam::sdk::Board::digital_value fake_digital_value();

}  // namespace board
}  // namespace sdktests
}  // namespace viam
