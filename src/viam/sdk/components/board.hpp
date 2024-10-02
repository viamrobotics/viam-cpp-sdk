/// @file components/board.hpp
///
/// @brief Defines a `Board` component.
#pragma once

#include <chrono>
#include <string>
#include <unordered_map>

#include <viam/api/component/board/v1/board.pb.h>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>

namespace viam {
namespace sdk {

/// @class Board board.hpp "components/board.hpp"
/// @brief Represents a physical board with gpio pins, digital interrupts, and analog voltage
/// reading
/// @ingroup Board
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific board implementations. This class cannot be used on its own.
class Board : public Component {
   public:
    /// @brief Represents the raw value received by the registered analog to digital converter
    /// (ADC). The range and conversion mechanism to voltage will vary depending on the specific
    /// ADC registered to the pin.
    using analog_value = int32_t;

    /// @brief Represents the response received when reading the registered analog to digital
    /// converter (ADC). The range and conversion mechanism to voltage will vary depending on the
    /// specific ADC registered to the pin, though the min and max voltages and step_size can often
    /// help with this conversion. Consult your ADC's documentation and Viam's `Board`
    /// documentation for more details.
    struct analog_response {
        analog_value value;
        float min_range;  // Minimum possible voltage read by the analog reader
        float max_range;  // Maximum possible voltage read by the analog reader
        float step_size;  // Volts represented in each step in the value
    };

    /// @brief Depending on the type of digital interrupt, this can have different meanings. If a
    /// `basic` (default) interrupt is configured, then this is the total interrupt count. If a
    /// `servo` interrupt is configured this tracks the pulse width value. Consult Viam's `Board`
    /// documentation for more details.
    using digital_value = int64_t;

    /// @struct status
    /// @brief This contains all of the values for all of the registered analog readers and digital
    /// interrupts that have been registered on the board.
    struct status {
        std::unordered_map<std::string, analog_value> analog_reader_values;
        std::unordered_map<std::string, digital_value> digital_interrupt_values;
    };

    /// @struct tick
    /// @brief A board's digital interrupt.
    struct Tick {
        /// name of the digital interrupt pin.
        std::string pin_name;

        /// time in nanoseconds the tick occured. This does not represent an absolute time.
        std::chrono::nanoseconds time;

        /// bool high or low.
        bool high;
    };

    /// @enum power_mode
    /// @brief Power mode of the board
    /// The effect of these power modes depends on your physical board
    enum class power_mode : uint8_t { normal = 0, offline_deep = 1 };

    API api() const override;

    /// @brief Creates a `status` struct from its proto representation.
    static status from_proto(const viam::component::board::v1::Status& proto);

    /// @brief Creates a `power_mode` enum from its proto representation.
    static power_mode from_proto(viam::component::board::v1::PowerMode proto);

    /// @brief Converts a `status` struct to its proto representation.
    static viam::component::board::v1::Status to_proto(const status& status);

    /// @brief Converts a `power_mode` enum to its proto representation.
    static viam::component::board::v1::PowerMode to_proto(power_mode power_mode);

    /// @brief Gets the high/low state of the given pin on a board.
    /// @param pin board pin name
    /// @return high/low state of the given pin. High = on, low = off
    inline bool get_gpio(const std::string& pin) {
        return get_gpio(pin, {});
    }

    /// @brief Gets the high/low state of the given pin on a board.
    /// @param pin board pin name
    /// @param extra Any additional arguments to the method
    /// @return high/low state of the given pin. High = on, low = off
    virtual bool get_gpio(const std::string& pin, const ProtoStruct& extra) = 0;

    /// @brief Set the gpio high/low state of the given pin on a board
    /// @param high true if the pin should be set to high (on) or false if it should be low (off)
    inline void set_gpio(const std::string& pin, bool high) {
        return set_gpio(pin, high, {});
    }

    /// @brief Set the gpio high/low state of the given pin on a board
    /// @param high true if the pin should be set to high (on) or false if it should be low (off)
    /// @param extra Any additional arguments to the method
    virtual void set_gpio(const std::string& pin, bool high, const ProtoStruct& extra) = 0;

    /// @brief Gets the duty cycle of the given pin on a board.
    /// @param pin board pin name
    /// @return duty cycle percentage (0 to 1)
    inline double get_pwm_duty_cycle(const std::string& pin) {
        return get_pwm_duty_cycle(pin, {});
    }

    /// @brief Gets the duty cycle of the given pin on a board.
    /// @param pin board pin name
    /// @param extra Any additional arguments to the method
    /// @return duty cycle percentage (0 to 1)
    virtual double get_pwm_duty_cycle(const std::string& pin, const ProtoStruct& extra) = 0;

    /// @brief Sets the given pin of a board to the given duty cycle.
    /// @param pin board pin name
    /// @param duty_cycle_pct duty cycle percentage 0 to 1
    inline void set_pwm_duty_cycle(const std::string& pin, double duty_cycle_pct) {
        return set_pwm_duty_cycle(pin, duty_cycle_pct, {});
    }

    /// @brief Sets the given pin of a board to the given duty cycle.
    /// @param pin board pin name
    /// @param duty_cycle_pct duty cycle percentage 0 to 1
    /// @param extra Any additional arguments to the method
    virtual void set_pwm_duty_cycle(const std::string& pin,
                                    double duty_cycle_pct,
                                    const ProtoStruct& extra) = 0;

    /// @brief Gets the PWM frequency of the given pin on a board.
    /// @param pin board pin name
    inline uint64_t get_pwm_frequency(const std::string& pin) {
        return get_pwm_frequency(pin, {});
    }

    /// @brief Gets the PWM frequency of the given pin on a board.
    /// @param pin board pin name
    /// @param extra Any additional arguments to the method
    virtual uint64_t get_pwm_frequency(const std::string& pin, const ProtoStruct& extra) = 0;

    /// @brief Sets the given pin on a board to the given PWM frequency. 0 will use the board's
    /// default PWM frequency.
    /// @param pin board pin name
    /// @param frequency_hz frequency in hz (0 = use board default frequency)
    inline void set_pwm_frequency(const std::string& pin, uint64_t frequency_hz) {
        return set_pwm_frequency(pin, frequency_hz, {});
    }

    /// @brief Sets the given pin on a board to the given PWM frequency. 0 will use the board's
    /// default PWM frequency.
    /// @param pin board pin name
    /// @param frequency_hz frequency in hz (0 = use board default frequency)
    /// @param extra Any additional arguments to the method
    virtual void set_pwm_frequency(const std::string& pin,
                                   uint64_t frequency_hz,
                                   const ProtoStruct& extra) = 0;

    /// @brief Reads off the current value of an analog reader on a board. Consult your ADC's docs
    /// or Viam's `Board` docs for more information.
    /// @param analog_reader_name analog reader to read from
    inline analog_response read_analog(const std::string& analog_reader_name) {
        return read_analog(analog_reader_name, {});
    }

    /// @brief Reads off the current value of an analog reader on a board. Consult your ADC's docs
    /// or Viam's `Board` docs for more information.
    /// @param analog_reader_name analog reader to read from
    /// @param extra Any additional arguments to the method
    virtual analog_response read_analog(const std::string& analog_reader_name,
                                        const ProtoStruct& extra) = 0;

    /// @brief Writes the value to the analog writer of the board.
    /// @param pin the pin to write to
    /// @param value the value to set the pin to
    inline void write_analog(const std::string& pin, int value) {
        return write_analog(pin, value, {});
    }

    /// @brief Writes the value to the analog writer of the board.
    /// @param pin the pin to write to
    /// @param value the value to set the pin to
    /// @param extra any additional arguments to the method
    virtual void write_analog(const std::string& pin, int value, const ProtoStruct& extra) = 0;

    /// @brief Returns the current value of the interrupt which is based on the type of
    /// interrupt. Consult Viam's `Board` docs for more information.
    /// @param digital_interrupt_name digital interrupt to check
    inline digital_value read_digital_interrupt(const std::string& digital_interrupt_name) {
        return read_digital_interrupt(digital_interrupt_name, {});
    }

    /// @brief Returns the current value of the interrupt which is based on the type of interrupt.
    /// Consult Viam's `Board` docs for more information.
    /// @param digital_interrupt_name digital interrupt to check
    /// @param extra Any additional arguments to the method
    virtual digital_value read_digital_interrupt(const std::string& digital_interrupt_name,
                                                 const ProtoStruct& extra) = 0;

    /// @brief Returns a stream of digital interrupt ticks.
    /// @param digital_interrupt_names digital interrupts to stream
    /// @param tick_handler callback function to call when a tick occurs.
    /// This should return true to keep streaming ticks and false to indicate that the stream of
    /// ticks should terminate. The callback function should not be blocking.
    inline void stream_ticks(std::vector<std::string> const& digital_interrupt_names,
                             std::function<bool(Tick&& tick)> const& tick_handler) {
        return stream_ticks(digital_interrupt_names, tick_handler, {});
    }

    /// @brief Returns a stream of digital interrupt ticks.
    /// @param digital_interrupt_names digital interrupts to stream
    /// @param tick_handler callback function to call when a tick occurs.
    /// @param extra Any additional arguments to the method
    virtual void stream_ticks(std::vector<std::string> const& digital_interrupt_names,
                              std::function<bool(Tick&& tick)> const& tick_handler,
                              const ProtoStruct& extra) = 0;

    /// @brief Sets the power consumption mode of the board to the requested setting for the given
    /// duration.
    /// @param power_mode Requested power mode
    /// @param duration Requested duration to stay in `power_mode` (in microseconds)
    inline void set_power_mode(power_mode power_mode,
                               const boost::optional<std::chrono::microseconds>& duration = {}) {
        return set_power_mode(power_mode, {}, duration);
    }

    /// @brief Sets the power consumption mode of the board to the requested setting for the given
    /// duration.
    /// @param power_mode Requested power mode
    /// @param extra Any additional arguments to the method
    /// @param duration Requested duration to stay in `power_mode` (in microseconds)
    virtual void set_power_mode(
        power_mode power_mode,
        const ProtoStruct& extra,
        const boost::optional<std::chrono::microseconds>& duration = {}) = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual ProtoStruct do_command(const ProtoStruct& command) = 0;

    /// @brief Returns `GeometryConfig`s associated with the calling board.
    /// @return The requested `GeometryConfig`s associated with the component.
    inline std::vector<GeometryConfig> get_geometries() {
        return get_geometries({});
    }

    /// @brief Returns `GeometryConfig`s associated with the calling board.
    /// @param extra Any additional arguments to the method.
    /// @return The requested `GeometryConfig`s associated with the component.
    virtual std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) = 0;

   protected:
    explicit Board(std::string name);
};

template <>
struct API::traits<Board> {
    static API api();
};

bool operator==(const Board::status& lhs, const Board::status& rhs);

}  // namespace sdk
}  // namespace viam
