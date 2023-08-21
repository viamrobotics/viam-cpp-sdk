#pragma once

#include "viam/sdk/common/proto_type.hpp"
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
    Board::status get_status(const sdk::AttributeMap& extra) override;
    void set_gpio(const std::string& pin, bool high) override;
    void set_gpio(const std::string& pin, bool high, const sdk::AttributeMap& extra) override;
    bool get_gpio(const std::string& pin) override;
    bool get_gpio(const std::string& pin, const sdk::AttributeMap& extra) override;
    double get_pwm_duty_cycle(const std::string& pin) override;
    double get_pwm_duty_cycle(const std::string& pin, const sdk::AttributeMap& extra) override;
    void set_pwm_duty_cycle(const std::string& pin, double duty_cycle_pct) override;
    void set_pwm_duty_cycle(const std::string& pin,
                            double duty_cycle_pct,
                            const sdk::AttributeMap& extra) override;
    uint64_t get_pwm_frequency(const std::string& pin) override;
    uint64_t get_pwm_frequency(const std::string& pin, const sdk::AttributeMap& extra) override;
    void set_pwm_frequency(const std::string& pin, uint64_t frequency_hz) override;
    void set_pwm_frequency(const std::string& pin,
                           uint64_t frequency_hz,
                           const sdk::AttributeMap& extra) override;
    viam::sdk::AttributeMap do_command(const viam::sdk::AttributeMap& command) override;
    Board::analog_value read_analog(const std::string& analog_reader_name) override;
    Board::analog_value read_analog(const std::string& analog_reader_name,
                                    const sdk::AttributeMap& extra) override;
    Board::digital_value read_digital_interrupt(const std::string& digital_interrupt_name) override;
    Board::digital_value read_digital_interrupt(const std::string& digital_interrupt_name,
                                                const sdk::AttributeMap& extra) override;
    void set_power_mode(power_mode power_mode,
                        const boost::optional<std::chrono::microseconds>& duration) override;
    void set_power_mode(power_mode power_mode,
                        const sdk::AttributeMap& extra,
                        const boost::optional<std::chrono::microseconds>& duration) override;
    std::vector<sdk::GeometryConfig> get_geometries() override;
    std::vector<sdk::GeometryConfig> get_geometries(const sdk::AttributeMap& extra) override;

    std::string peek_pin, peek_analog_reader_name, peek_digital_interrupt_name;
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
    boost::optional<std::chrono::microseconds> peek_set_power_mode_duration;
};

}  // namespace board
}  // namespace sdktests
}  // namespace viam
