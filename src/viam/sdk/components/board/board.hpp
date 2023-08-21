/// @file components/board/board.hpp
///
/// @brief Defines a `Board` component.
#pragma once

#include <string>
#include <unordered_map>

#include <viam/api/component/board/v1/board.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>

namespace viam {
namespace sdk {

/// @defgroup Board Classes related to the Board component.

/// @class BoardRegistration
/// @brief Defines a `ResourceRegistration` for the `Board` component.
/// @ingroup Board
class BoardRegistration : public ResourceRegistration {
   public:
    explicit BoardRegistration(const google::protobuf::ServiceDescriptor* service_descriptor);
    std::shared_ptr<ResourceServer> create_resource_server(
        std::shared_ptr<ResourceManager> manager) override;
    std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                std::shared_ptr<grpc::Channel> chan) override;
};

/// @class Board board.hpp "components/board/board.hpp"
/// @brief Represents a physical board with gpio pins, digital interrupts, and analog voltage
/// reading
/// @ingroup Board
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific board implementations. This class cannot be used on its own.
class Board : public Component {
   public:
    /// @brief Represents the value received by the registered analog to digital converter (ADC).
    /// The range and conversion mechanism to voltage will vary depending on the specific ADC
    /// registered to the pin. Consult your ADC's documentation and Viam's `Board` documentation for
    /// more details.
    using analog_value = int32_t;

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

    /// @enum power_mode
    /// @brief Power mode of the board
    /// The effect of these power modes depends on your physical board
    enum class power_mode : uint8_t { normal = 0, offline_deep = 1 };

    // functions shared across all components
    static std::shared_ptr<ResourceRegistration> resource_registration();
    static API static_api();
    API dynamic_api() const override;

    /// @brief Creates a `status` struct from its proto representation.
    static status from_proto(viam::common::v1::BoardStatus proto);

    /// @brief Creates a `analog_value` struct from its proto representation.
    static analog_value from_proto(viam::common::v1::AnalogStatus proto);

    /// @brief Creates a `digital_value` struct from its proto representation.
    static digital_value from_proto(viam::common::v1::DigitalInterruptStatus proto);

    /// @brief Creates a `power_mode` enum from its proto representation.
    static power_mode from_proto(viam::component::board::v1::PowerMode proto);

    /// @brief Converts a `status` struct to its proto representation.
    static viam::common::v1::BoardStatus to_proto(status status);

    /// @brief Converts a `analog_value` struct to its proto representation.
    static viam::common::v1::AnalogStatus to_proto(analog_value analog_value);

    /// @brief Converts a `digital_value` struct to its proto representation.
    static viam::common::v1::DigitalInterruptStatus to_proto(digital_value digital_value);

    /// @brief Converts a `power_mode` enum to its proto representation.
    static viam::component::board::v1::PowerMode to_proto(power_mode power_mode);

    /// @brief Get the status of all of the registered analog readers and digital interrupt readers
    virtual status get_status() = 0;

    /// @brief Get the status of all of the registered analog readers and digital interrupt readers
    /// @param extra Any additional arguments to the method
    virtual status get_status(const AttributeMap& extra) = 0;

    /// @brief Get the names of the defined analog readers defined for this board
    /// This information comes from calling `get_status()`
    std::vector<std::string> get_analog_reader_names();

    /// @brief Get the names of the defined digital interrupts for this board
    /// This information comes from calling `get_status()`
    std::vector<std::string> get_digital_interrupt_names();

    /// @brief Get all defined analog readers defined for this board
    /// This information comes from calling `get_status()`
    std::unordered_map<std::string, analog_value> get_analog_readers();

    /// @brief Get all defined digital interrupts for this board
    /// This information comes from calling `get_status()`
    std::unordered_map<std::string, digital_value> get_digital_interrupts();

    /// @brief Gets the high/low state of the given pin on a board.
    /// @param pin board pin name
    /// @return high/low state of the given pin. High = on, low = off
    virtual bool get_gpio(const std::string& pin) = 0;

    /// @brief Gets the high/low state of the given pin on a board.
    /// @param pin board pin name
    /// @param extra Any additional arguments to the method
    /// @return high/low state of the given pin. High = on, low = off
    virtual bool get_gpio(const std::string& pin, const AttributeMap& extra) = 0;

    /// @brief Set the gpio high/low state of the given pin on a board
    /// @param high true if the pin should be set to high (on) or false if it should be low (off)
    virtual void set_gpio(const std::string& pin, bool high) = 0;

    /// @brief Set the gpio high/low state of the given pin on a board
    /// @param high true if the pin should be set to high (on) or false if it should be low (off)
    /// @param extra Any additional arguments to the method
    virtual void set_gpio(const std::string& pin, bool high, const AttributeMap& extra) = 0;

    /// @brief Gets the duty cycle of the given pin on a board.
    /// @param pin board pin name
    /// @return duty cycle percentage (0 to 1)
    virtual double get_pwm_duty_cycle(const std::string& pin) = 0;

    /// @brief Gets the duty cycle of the given pin on a board.
    /// @param pin board pin name
    /// @param extra Any additional arguments to the method
    /// @return duty cycle percentage (0 to 1)
    virtual double get_pwm_duty_cycle(const std::string& pin, const AttributeMap& extra) = 0;

    /// @brief Sets the given pin of a board to the given duty cycle.
    /// @param pin board pin name
    /// @param duty_cycle_pct duty cycle percentage 0 to 1
    virtual void set_pwm_duty_cycle(const std::string& pin, double duty_cycle_pct) = 0;

    /// @brief Sets the given pin of a board to the given duty cycle.
    /// @param pin board pin name
    /// @param duty_cycle_pct duty cycle percentage 0 to 1
    /// @param extra Any additional arguments to the method
    virtual void set_pwm_duty_cycle(const std::string& pin,
                                    double duty_cycle_pct,
                                    const AttributeMap& extra) = 0;

    /// @brief Gets the PWM frequency of the given pin on a board.
    /// @param pin board pin name
    virtual uint64_t get_pwm_frequency(const std::string& pin) = 0;

    /// @brief Gets the PWM frequency of the given pin on a board.
    /// @param pin board pin name
    /// @param extra Any additional arguments to the method
    virtual uint64_t get_pwm_frequency(const std::string& pin, const AttributeMap& extra) = 0;

    /// @brief Sets the given pin on a board to the given PWM frequency. 0 will use the board's
    /// default PWM frequency.
    /// @param pin board pin name
    /// @param frequency_hz frequency in hz (0 = use board default frequency)
    virtual void set_pwm_frequency(const std::string& pin, uint64_t frequency_hz) = 0;

    /// @brief Sets the given pin on a board to the given PWM frequency. 0 will use the board's
    /// default PWM frequency.
    /// @param pin board pin name
    /// @param frequency_hz frequency in hz (0 = use board default frequency)
    /// @param extra Any additional arguments to the method
    virtual void set_pwm_frequency(const std::string& pin,
                                   uint64_t frequency_hz,
                                   const AttributeMap& extra) = 0;

    /// @brief Reads off the current value of an analog reader on a board. Consult your ADC's docs
    /// or Viam's `Board` docs for more information.
    /// @param analog_reader_name analog reader to read from
    virtual analog_value read_analog(const std::string& analog_reader_name) = 0;

    /// @brief Reads off the current value of an analog reader on a board. Consult your ADC's docs
    /// or Viam's `Board` docs for more information.
    /// @param analog_reader_name analog reader to read from
    /// @param extra Any additional arguments to the method
    virtual analog_value read_analog(const std::string& analog_reader_name,
                                     const AttributeMap& extra) = 0;

    /// @brief Returns the current value of the interrupt which is based on the type of interrupt.
    /// Consult Viam's `Board` docs for more information.
    /// @param digital_interrupt_name digital interrupt to check
    virtual digital_value read_digital_interrupt(const std::string& digital_interrupt_name) = 0;

    /// @brief Returns the current value of the interrupt which is based on the type of interrupt.
    /// Consult Viam's `Board` docs for more information.
    /// @param digital_interrupt_name digital interrupt to check
    /// @param extra Any additional arguments to the method
    virtual digital_value read_digital_interrupt(const std::string& digital_interrupt_name,
                                                 const AttributeMap& extra) = 0;

    /// @brief Sets the power consumption mode of the board to the requested setting for the given
    /// duration.
    /// @param power_mode Requested power mode
    /// @param duration Requested duration to stay in `power_mode` (in microseconds)
    virtual void set_power_mode(
        power_mode power_mode, const boost::optional<std::chrono::microseconds>& duration = {}) = 0;

    /// @brief Sets the power consumption mode of the board to the requested setting for the given
    /// duration.
    /// @param power_mode Requested power mode
    /// @param extra Any additional arguments to the method
    /// @param duration Requested duration to stay in `power_mode` (in microseconds)
    virtual void set_power_mode(
        power_mode power_mode,
        const AttributeMap& extra,
        const boost::optional<std::chrono::microseconds>& duration = {}) = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(const AttributeMap& command) = 0;

    /// @brief Returns `GeometryConfig`s associated with the calling board.
    /// @return The requested `GeometryConfig`s associated with the component.
    virtual std::vector<GeometryConfig> get_geometries() = 0;

    /// @brief Returns `GeometryConfig`s associated with the calling board.
    /// @param extra Any additional arguments to the method.
    /// @return The requested `GeometryConfig`s associated with the component.
    virtual std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) = 0;

   protected:
    explicit Board(std::string name);
};

bool operator==(const Board::status& lhs, const Board::status& rhs);

}  // namespace sdk
}  // namespace viam
