/// @file components/board/board.hpp
///
/// @brief Defines a `Board` component.
#pragma once

#include <string>

#include <viam/api/component/board/v1/board.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>

namespace viam {
namespace sdk {

/// @defgroup Board Classes related to the `Board` component.

/// @class BoardSubtype
/// @brief Defines a `ResourceSubtype` for the `Board` component.
/// @ingroup Board
class BoardSubtype : public ResourceSubtype {
   public:
    std::shared_ptr<ResourceServerBase> create_resource_server(
        std::shared_ptr<ResourceManager> manager) override;
    std::shared_ptr<ResourceBase> create_rpc_client(std::string name,
                                                    std::shared_ptr<grpc::Channel> chan) override;
    BoardSubtype(const google::protobuf::ServiceDescriptor* service_descriptor)
        : ResourceSubtype(service_descriptor){};
};

/// @class Board board.hpp "components/board/board.hpp"
/// @brief TODO
/// @ingroup Board
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific board implementations. This class cannot be used on its own.
class Board : public ComponentBase {
   public:
    /// @struct gpio_pin
    /// @brief TODO.
    struct gpio_pin {
        /// TODO
        bool high;
    };

    /// @struct duty_cycle
    /// @brief TODO.
    struct duty_cycle {
        /// TODO
        double duty_cycle_pct;
    };

    /// @struct pwm_frequency
    /// @brief TODO.
    struct pwm_frequency {
        /// TODO
        uint64_t frequency_hz;
    };

    // TODO
    typedef int32_t analog_value;

    // TODO
    typedef int64_t digital_value;

    /// @struct status
    /// @brief TODO.
    struct status {
        std::map<std::string, analog_value> analog_values;
        std::map<std::string, digital_value> digital_values;
    };

    /// @enum power_mode
    /// @brief TODO.
    enum power_mode { POWER_MODE_UNSPECIFIED, POWER_MODE_NORMAL, POWER_MODE_OFFLINE_DEEP };

    // functions shared across all components
    static std::shared_ptr<ResourceSubtype> resource_subtype();
    static Subtype subtype();

    /// @brief Creates a `status` struct from its proto representation.
    static status from_proto(viam::common::v1::BoardStatus proto);

    /// @brief Creates a `gpio_pin` struct from its proto representation.
    static gpio_pin from_proto(viam::component::board::v1::GetGPIOResponse proto);

    /// @brief Creates a `duty_cycle` struct from its proto representation.
    static duty_cycle from_proto(viam::component::board::v1::PWMResponse proto);

    /// @brief Creates a `analog_value` struct from its proto representation.
    static analog_value from_proto(viam::common::v1::AnalogStatus proto);

    /// @brief Creates a `digital_value` struct from its proto representation.
    static digital_value from_proto(viam::common::v1::DigitalInterruptStatus proto);

    /// @brief Creates a `power_mode` enum from its proto representation.
    static power_mode from_proto(viam::component::board::v1::PowerMode proto);

    /// @brief Creates a `power_mode` enum from its proto representation.
    static std::chrono::duration<double> from_proto(google::protobuf::Duration proto);

    /// @brief Converts a `status` struct to its proto representation.
    static viam::common::v1::BoardStatus to_proto(status status);

    /// @brief Converts a `gpio_pin` struct to its proto representation.
    static viam::component::board::v1::GetGPIOResponse to_proto(gpio_pin gpio_pin);

    /// @brief Converts a `duty_cycle` struct to its proto representation.
    static viam::component::board::v1::PWMResponse to_proto(duty_cycle duty_cycle);

    /// @brief Converts a `analog_value` struct to its proto representation.
    static viam::common::v1::AnalogStatus to_proto(analog_value analog_value);

    /// @brief Converts a `digital_value` struct to its proto representation.
    static viam::common::v1::DigitalInterruptStatus to_proto(digital_value digital_value);

    /// @brief Converts a `power_mode` enum to its proto representation.
    static viam::component::board::v1::PowerMode to_proto(power_mode power_mode);

    /// @brief Converts a `duration` struct to its proto representation.
    static google::protobuf::Duration to_proto(std::chrono::duration<double> duration);

    /// @brief TODO
    virtual status get_status() = 0;

    /// @brief TODO
    /// @param pin TODO
    /// @param high TODO
    virtual void set_gpio(std::string pin, bool high) = 0;

    /// @brief GetGPIO gets the high/low state of the given pin of a board of the underlying robot.
    /// @param pin TODO
    virtual gpio_pin get_gpio(std::string pin) = 0;

    /// @brief PWM gets the duty cycle of the given pin of a board of the underlying robot.
    /// @param pin TODO
    virtual duty_cycle get_pwm(std::string pin) = 0;

    /// @brief SetPWM sets the given pin of a board of the underlying robot to the given duty cycle.
    /// @param pin TODO
    /// @param duty_cycle_pct TODO
    virtual void set_pwm(std::string pin, double duty_cycle_pct) = 0;

    /// @brief PWMFrequency gets the PWM frequency of the given pin of a board of the underlying
    /// robot.
    /// @param pin TODO
    virtual uint64_t get_pwm_frequency(std::string pin) = 0;

    /// @brief SetPWMFrequency sets the given pin of a board of the underlying robot to the given
    /// PWM frequency. 0 will use the board's default PWM frequency.
    /// @param pin TODO
    /// @param frequency_hz TODO
    virtual void set_pwm_frequency(std::string pin, uint64_t frequency_hz) = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(AttributeMap command) = 0;

    /// @brief Analog Reader ReadAnalogReader reads off the current value of an analog reader of a
    /// board of the underlying robot.
    /// @param board_name TODO
    /// @param analog_reader_name TODO
    virtual analog_value read_analog(std::string analog_reader_name) = 0;

    /// @brief Digital Interrupt GetDigitalInterruptValue returns the current value of the interrupt
    /// which is based on the type of interrupt.
    /// @param board_name TODO
    /// @param digital_interrupt_name TODO
    virtual digital_value read_digital_interrupt(std::string digital_interrupt_name) = 0;

    /// @brief Power Management `SetPowerMode` sets the power consumption mode of the board to the
    /// requested setting for the given duration.
    /// @param power_mode Requested power mode
    /// @param duration Requested duration to stay in `power_mode`
    virtual void set_power_mode(power_mode power_mode, std::chrono::duration<double> duration) = 0;

   protected:
    explicit Board(std::string name) : ComponentBase(std::move(name)){};
};

bool operator==(const Board::status& lhs, const Board::status& rhs);
bool operator==(const Board::gpio_pin& lhs, const Board::gpio_pin& rhs);
bool operator==(const Board::duty_cycle& lhs, const Board::duty_cycle& rhs);

}  // namespace sdk
}  // namespace viam
