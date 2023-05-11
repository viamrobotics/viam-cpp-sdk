/// @file components/motor/motor.hpp
///
/// @brief Defines a `Motor` component.
#pragma once

#include <string>

#include <viam/api/component/motor/v1/motor.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>

namespace viam {
namespace sdk {

/// @defgroup Motor Classes related to the `Motor` component.

/// @class MotorRegistration
/// @brief Defines a `ResourceRegistration` for the `Motor` component.
/// @ingroup Motor
class MotorRegistration : public ResourceRegistration {
   public:
    std::shared_ptr<ResourceServer> create_resource_server(
        std::shared_ptr<ResourceManager> manager) override;
    std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                std::shared_ptr<grpc::Channel> chan) override;
    MotorRegistration(const google::protobuf::ServiceDescriptor* service_descriptor)
        : ResourceRegistration(service_descriptor){};
};

/// @class Motor motor.hpp "components/motor/motor.hpp"
/// @brief A `Motor` represents physical hardware that controls the rotation of an axle
/// @ingroup Motor
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific motor implementations. This class cannot be used on its own.
class Motor : public Component {
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

    // functions shared across all components
    static std::shared_ptr<ResourceRegistration> resource_registration();
    static API static_api();

    /// @brief Creates a `position` struct from its proto representation.
    static position from_proto(viam::component::motor::v1::GetPositionResponse proto);

    /// @brief Creates a `power_status` struct from its proto representation.
    static power_status from_proto(viam::component::motor::v1::IsPoweredResponse proto);

    /// @brief Creates a `properties` struct from its proto representation.
    static properties from_proto(viam::component::motor::v1::GetPropertiesResponse proto);

    /// @brief Converts a `position` struct to its proto representation.
    static viam::component::motor::v1::GetPositionResponse to_proto(position position);

    /// @brief Converts a `power_status` struct to its proto representation.
    static viam::component::motor::v1::IsPoweredResponse to_proto(power_status power_status);

    /// @brief Converts a `properties` struct to its proto representation.
    static viam::component::motor::v1::GetPropertiesResponse to_proto(properties properties);

    /// @brief Sets the percentage of the motor's total power that should be employed.
    /// @param power_pct Percentage of motor's power, between -1 and 1, where negative values
    /// indicate a backwards direction and positive values, a forward direction.
    virtual void set_power(double power_pct) = 0;

    /// @brief Instructs the motor to turn at a specified speed, which is expressed in RPM, for a
    /// specified number of rotations relative to its starting position.
    /// @param rpm Speed of motor travel in rotations per minute
    /// @param revolutions Number of revolutions relative to motor's start position. If
    /// `revolutions` == 0, this will run the motor at `rpm` indefinetely. If `revolutions` != 0,
    /// this will block until the number of revolutions has been completed or another operation
    /// comes in.
    /// @throws ViamException if position reporting is not supported
    virtual void go_for(double rpm, double revolutions) = 0;

    /// @brief Move the motor to a specific position that is relative to its
    /// home position at a specified speed which is expressed in RPM.
    /// @param rpm Speed of motor travel in rotations per minute
    /// @param position_revolutions Number of revolutions relative to motor's home home/zero
    /// @throws ViamException if position reporting is not supported
    virtual void go_to(double rpm, double position_revolutions) = 0;

    /// @brief Sets the current position of the motor as the new zero position.
    /// @param offset Motor position
    /// @throws ViamException if position reporting is not supported
    virtual void reset_zero_position(double offset) = 0;

    /// @brief Reports the position of the robot's motor relative to its zero position.
    /// @throws ViamException if position reporting is not supported
    virtual position get_position() = 0;

    /// @brief Returns the properties of the motor which comprises the booleans indicating
    /// which optional features the robot's motor supports
    virtual properties get_properties() = 0;

    /// @return The motor's current power_status
    virtual power_status get_power_status() = 0;

    /// @brief Reports if a component is in motion
    virtual bool is_moving() = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(AttributeMap command) = 0;

    API dynamic_api() const override;

   protected:
    explicit Motor(std::string name) : Component(std::move(name)){};
};

bool operator==(const Motor::power_status& lhs, const Motor::power_status& rhs);
bool operator==(const Motor::properties& lhs, const Motor::properties& rhs);

}  // namespace sdk
}  // namespace viam
