/// @file components/motor/motor.hpp
///
/// @brief Defines a `Motor` component.
#pragma once

#include <common/proto_type.hpp>
#include <common/utils.hpp>
#include <component/motor/v1/motor.pb.h>
#include <config/resource.hpp>
#include <registry/registry.hpp>
#include <string>
#include <subtype/subtype.hpp>

/// @defgroup Motor Classes related to the `Motor` component.

/// @class MotorSubtype
/// @brief Defines a `ResourceSubtype` for the `Motor` component.
/// @ingroup Motor
class MotorSubtype : public ResourceSubtype {
public:
  std::shared_ptr<ResourceServerBase> create_resource_server(
      std::shared_ptr<SubtypeService> svc) override;
  std::shared_ptr<ResourceBase> create_rpc_client(
      std::string name, std::shared_ptr<grpc::Channel> chan) override;
  MotorSubtype(const google::protobuf::ServiceDescriptor *service_descriptor)
      : ResourceSubtype(service_descriptor){};
};

/// @class Motor motor.hpp "components/motor/motor.hpp"
/// @brief TODO
/// @ingroup Motor
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific motor implementations. This class cannot be used on its own.
class Motor : public ComponentBase {
public:
  
  /// @struct position
  /// @brief TODO.
  struct position {
        /// Current position of the motor relative to its home
        double position;
  };
  
  /// @struct power_status
  /// @brief TODO.
  struct power_status {
        /// Returns true if the motor is on
        bool is_on;
        /// Returns power percent (from 0 to 1, or from -1 to 1 for motors that support negative power), based on the last command sent to motor. If the last command was a stop command, this value will be 0.
        double power_pct;
  };
  
  /// @struct properties
  /// @brief TODO.
  struct properties {
        /// Returns true if the motor supports reporting its position
        bool position_reporting;
  };
  
  /// @struct moving_status
  /// @brief TODO.
  struct moving_status {
        ///
        bool is_moving;
  };
  
  

  // functions shared across all components
  static std::shared_ptr<ResourceSubtype> resource_subtype();
  static Subtype subtype();

  
  /// @brief creates an `position` struct from its proto representation.
  static position from_proto(viam::component::motor::v1::GetPositionResponse proto);
    
  /// @brief creates an `power_status` struct from its proto representation.
  static power_status from_proto(viam::component::motor::v1::IsPoweredResponse proto);
    
  /// @brief creates an `properties` struct from its proto representation.
  static properties from_proto(viam::component::motor::v1::GetPropertiesResponse proto);
    
  /// @brief creates an `moving_status` struct from its proto representation.
  static moving_status from_proto(viam::component::motor::v1::IsMovingResponse proto);
    

  
  /// @brief converts an `position` struct to its proto representation.
  static viam::component::motor::v1::GetPositionResponse to_proto(position position);
  
  /// @brief converts an `power_status` struct to its proto representation.
  static viam::component::motor::v1::IsPoweredResponse to_proto(power_status power_status);
  
  /// @brief converts an `properties` struct to its proto representation.
  static viam::component::motor::v1::GetPropertiesResponse to_proto(properties properties);
  
  /// @brief converts an `moving_status` struct to its proto representation.
  static viam::component::motor::v1::IsMovingResponse to_proto(moving_status moving_status);
  

  
  ///  SetPower sets the percentage of the motor's total power that should be employed expressed a value between -1 and 1 where negative values indicate a backwards direction and positive values a forward direction
  virtual void set_power(double power_pct) = 0;
  
  ///  GoFor instructs the motor to turn at a specified speed, which is expressed in RPM, for a specified number of rotations relative to its starting position This method will return an error if position reporting is not supported If revolutions is 0, this will run the motor at rpm indefinitely If revolutions != 0, this will block until the number of revolutions has been completed or another operation comes in.
  virtual void go_for(double rpm, double revolutions) = 0;
  
  ///  GoTo requests the robot's motor to move to a specific position that is relative to its home position at a specified speed which is expressed in RPM This method will return an error if position reporting is not supported
  virtual void go_to(double rpm, double position_revolutions) = 0;
  
  ///  ResetZeroPosition sets the current position of the motor as the new zero position This method will return an error if position reporting is not supported
  virtual void reset_zero_position(double offset) = 0;
  
  ///  Position reports the position of the robot's motor relative to its zero position This method will return an error if position reporting is not supported
  virtual position get_position() = 0;
  
  ///  GetProperties returns a message of booleans indicating which optional features the robot's motor supports
  virtual properties get_properties() = 0;
  
  ///  Stop turns the robot's motor off
  virtual void stop() = 0;
  
  ///  IsPowered returns true if the robot's motor is on
  virtual power_status get_power_status() = 0;
  
  ///  IsMoving reports if a component is in motion
  virtual moving_status is_moving() = 0;
  
  ///  DoCommand sends/receives arbitrary commands
  virtual AttributeMap do_command(ERROR TODO) = 0;
  

protected:
  explicit Motor(std::string name) : ComponentBase(std::move(name)){};
};


bool operator==(const Motor::position& lhs, const Motor::position& rhs);

bool operator==(const Motor::power_status& lhs, const Motor::power_status& rhs);

bool operator==(const Motor::properties& lhs, const Motor::properties& rhs);

bool operator==(const Motor::moving_status& lhs, const Motor::moving_status& rhs);

