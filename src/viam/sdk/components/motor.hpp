/// @file components/motor.hpp
///
/// @brief Defines a `Motor` component.
#pragma once

#include <string>

#include <viam/api/component/motor/v1/motor.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/stoppable.hpp>

namespace viam {
namespace sdk {

/// @class Motor motor.hpp "components/motor.hpp"
/// @brief A `Motor` represents physical hardware that controls the rotation of an axle
/// @ingroup Motor
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific motor implementations. This class cannot be used on its own.
class Motor : public Component, public Stoppable {
   public:
    /// @struct position
    /// @brief Current position of the motor relative to its home
    typedef double position;

    /// @struct power_status
    /// @brief Information about power-state of the motor
    struct power_status {
        /// True if the motor is on
        bool is_on;
        /// Power percent (from 0 to 1, or from -1 to 1 for motors that support negative
        /// power), based on the last command sent to motor. If the last command was a stop command,
        /// this value will be 0.
        double power_pct;
    };

    /// @struct properties
    /// @brief Features that the specific motor supports
    struct properties {
        /// True if the motor supports reporting its position
        bool position_reporting;
    };

    /// @brief Creates a `position` struct from its proto representation.
    static position from_proto(const viam::component::motor::v1::GetPositionResponse& proto);

    /// @brief Creates a `power_status` struct from its proto representation.
    static power_status from_proto(const viam::component::motor::v1::IsPoweredResponse& proto);

    /// @brief Creates a `properties` struct from its proto representation.
    static properties from_proto(const viam::component::motor::v1::GetPropertiesResponse& proto);

    /// @brief Converts a `position` struct to its proto representation.
    static viam::component::motor::v1::GetPositionResponse to_proto(const position& position);

    /// @brief Converts a `power_status` struct to its proto representation.
    static viam::component::motor::v1::IsPoweredResponse to_proto(const power_status& power_status);

    /// @brief Converts a `properties` struct to its proto representation.
    static viam::component::motor::v1::GetPropertiesResponse to_proto(const properties& properties);

    /// @brief Sets the percentage of the motor's total power that should be employed.
    /// @param power_pct Percentage of motor's power, between -1 and 1, where negative values
    /// indicate a backwards direction and positive values, a forward direction.
    inline void set_power(double power_pct) {
        return set_power(power_pct, {});
    }

    /// @brief Sets the percentage of the motor's total power that should be employed.
    /// @param power_pct Percentage of motor's power, between -1 and 1, where negative values
    /// indicate a backwards direction and positive values, a forward direction.
    /// @param extra Any additional arguments to the method.
    virtual void set_power(double power_pct, const AttributeMap& extra) = 0;

    /// @brief Instructs the motor to turn at a specified speed, which is expressed in RPM, for a
    /// specified number of rotations relative to its starting position.
    /// @param rpm Speed of motor travel in rotations per minute
    /// @param revolutions Number of revolutions relative to motor's start position. If
    /// `revolutions` == 0, this will run the motor at `rpm` indefinetely. If `revolutions` != 0,
    /// this will block until the number of revolutions has been completed or another operation
    /// comes in.
    /// @throws `Exception` if position reporting is not supported
    inline void go_for(double rpm, double revolutions) {
        return go_for(rpm, revolutions, {});
    }

    /// @brief Instructs the motor to turn at a specified speed, which is expressed in RPM, for a
    /// specified number of rotations relative to its starting position.
    /// @param rpm Speed of motor travel in rotations per minute
    /// @param revolutions Number of revolutions relative to motor's start position. If
    /// `revolutions` == 0, this will run the motor at `rpm` indefinetely. If `revolutions` != 0,
    /// this will block until the number of revolutions has been completed or another operation
    /// comes in.
    /// @param extra Any additional arguments to the method.
    /// @throws `Exception` if position reporting is not supported
    virtual void go_for(double rpm, double revolutions, const AttributeMap& extra) = 0;

    /// @brief Move the motor to a specific position that is relative to its
    /// home position at a specified speed which is expressed in RPM.
    /// @param rpm Speed of motor travel in rotations per minute
    /// @param position_revolutions Number of revolutions relative to motor's home home/zero
    /// @throws `Exception` if position reporting is not supported
    inline void go_to(double rpm, double position_revolutions) {
        return go_to(rpm, position_revolutions, {});
    }

    /// @brief Move the motor to a specific position that is relative to its
    /// home position at a specified speed which is expressed in RPM.
    /// @param rpm Speed of motor travel in rotations per minute
    /// @param position_revolutions Number of revolutions relative to motor's home home/zero
    /// @param extra Any additional arguments to the method.
    /// @throws `Exception` if position reporting is not supported
    virtual void go_to(double rpm, double position_revolutions, const AttributeMap& extra) = 0;

    /// @brief Move the motor indefinitely at a specified speed which is expressed in RPM.
    /// @param rpm Speed of motor travel in rotations per minute
    /// @throws `Exception` if position reporting is not supported
    inline void set_rpm(double rpm) {
        return set_rpm(rpm, {});
    }

    /// @brief Move the motor indefinitely at a specified speed which is expressed in RPM.
    /// @param rpm Speed of motor travel in rotations per minute
    /// @param extra Any additional arguments to the method.
    /// @throws `Exception` if position reporting is not supported
    virtual void set_rpm(double rpm, const AttributeMap& extra) = 0;

    /// @brief Sets the current position of the motor as the new zero position.
    /// @param offset Motor position
    /// @throws `Exception` if position reporting is not supported
    inline void reset_zero_position(double offset) {
        return reset_zero_position(offset, {});
    }

    /// @brief Sets the current position of the motor as the new zero position.
    /// @param offset Motor position
    /// @param extra Any additional arguments to the method
    /// @throws `Exception` if position reporting is not supported
    virtual void reset_zero_position(double offset, const AttributeMap& extra) = 0;

    /// @brief Reports the position of the robot's motor relative to its zero position.
    /// @throws `Exception` if position reporting is not supported
    inline position get_position() {
        return get_position({});
    }

    /// @brief Reports the position of the robot's motor relative to its zero position.
    /// @param extra Any additional arguments to the method
    /// @throws `Exception` if position reporting is not supported
    virtual position get_position(const AttributeMap& extra) = 0;

    /// @brief Returns the properties of the motor which comprises the booleans indicating
    /// which optional features the robot's motor supports
    inline properties get_properties() {
        return get_properties({});
    }

    /// @brief Returns the properties of the motor which comprises the booleans indicating
    /// @param extra Any additional arguments to the method
    /// which optional features the robot's motor supports
    virtual properties get_properties(const AttributeMap& extra) = 0;

    /// @return The motor's current power_status
    inline power_status get_power_status() {
        return get_power_status({});
    }

    /// @return The motor's current power_status
    /// @param extra Any additional arguments to the method
    virtual power_status get_power_status(const AttributeMap& extra) = 0;

    /// @brief Reports if a component is in motion
    virtual bool is_moving() = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(const AttributeMap& command) = 0;

    /// @brief Returns `GeometryConfig`s associated with the calling motor.
    /// @return The requested `GeometryConfig`s associated with the component.
    inline std::vector<GeometryConfig> get_geometries() {
        return get_geometries({});
    }

    /// @brief Returns `GeometryConfig`s associated with the calling motor.
    /// @param extra Any additional arguments to the method.
    /// @return The requested `GeometryConfig`s associated with the component.
    virtual std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) = 0;

    API api() const override;

   protected:
    explicit Motor(std::string name);
};

template <>
struct API::traits<Motor> {
    static API api();
};

bool operator==(const Motor::power_status& lhs, const Motor::power_status& rhs);
bool operator==(const Motor::properties& lhs, const Motor::properties& rhs);

}  // namespace sdk
}  // namespace viam
