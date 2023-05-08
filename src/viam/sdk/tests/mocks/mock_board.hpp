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
    MockBoard(std::string name);
    Board::status get_status() override;
    void set_gpio(const std::string& pin, bool high) override;
    bool get_gpio(const std::string& pin) override;
    double get_pwm_duty_cycle(const std::string& pin) override;
    void set_pwm_duty_cycle(const std::string& pin, double duty_cycle_pct) override;
    uint64_t get_pwm_frequency(const std::string& pin) override;
    void set_pwm_frequency(const std::string& pin, uint64_t frequency_hz) override;
    viam::sdk::AttributeMap do_command(viam::sdk::AttributeMap command) override;
    Board::analog_value read_analog(const std::string& analog_reader_name) override;
    Board::digital_value read_digital_interrupt(const std::string& digital_interrupt_name) override;
    void set_power_mode(
        power_mode power_mode,
        boost::optional<const std::chrono::duration<int64_t, std::micro>&> duration) override;
    std::vector<std::string> get_analog_reader_names() override;
    std::vector<std::string> get_digital_interrupt_names() override;

    std::string peek_pin;
    Board::status peek_get_status_ret;
    bool peek_set_gpio_high;
    bool peek_get_gpio_ret;
    double peek_get_pwm_duty_cycle_ret;
    double peek_set_pwm_duty_cycle_pct;
    uint64_t peek_get_pwm_frequency_ret;
    uint64_t peek_set_pwm_frequency_hz;
    Board::analog_value peek_read_analog_ret;
    Board::digital_value peek_read_digital_interrupt_ret;
    Board::power_mode peek_set_power_mode_power_mode;
    boost::optional<std::chrono::duration<int64_t, std::micro>> peek_set_power_mode_duration;
};

}  // namespace board
}  // namespace sdktests
}  // namespace viam
