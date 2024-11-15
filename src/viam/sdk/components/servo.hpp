/// @file components/servo.hpp
///
/// @brief Defines a `Servo` component.
#pragma once

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/stoppable.hpp>

namespace viam {
namespace sdk {

/// @defgroup Servo Classes related to the Servo component.

/// @class Servo servo.hpp "components/servo.hpp"
/// @ingroup Servo
///
/// This acts as an abstract base class to be inherited form by any drivers representing
/// specific servo implementations. This class cannot be used on its own.
class Servo : public Component, public Stoppable {
   public:
    /// @struct position
    /// @brief Current position of the servo relative to its home
    typedef uint32_t position;

    API api() const override;

    /// @brief Move the servo to the provided angle
    /// @param angle_deg The desired angle of the servo in degrees.
    inline void move(uint32_t angle_deg) {
        return move(angle_deg, {});
    }

    /// @brief Move the servo to the provided angle
    /// @param angle_deg The desired angle of the servo in degrees.
    /// @param extra Any additional arguments to the method.
    virtual void move(uint32_t angle_deg, const ProtoStruct& extra) = 0;

    /// @brief Get the current angle (degrees) of the servo.
    /// @throws `Exception` if position reporting is not supported
    inline position get_position() {
        return get_position({});
    }

    /// @brief Reports the position of the robot's servo relative to its zero position.
    /// @param extra Any additional arguments to the method
    /// @throws `Exception` if position reporting is not supported
    virtual position get_position(const ProtoStruct& extra) = 0;

    /// @brief Reports if a component is in motion
    virtual bool is_moving() = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual ProtoStruct do_command(const ProtoStruct& command) = 0;

    /// @brief Returns `GeometryConfig`s associated with the calling servo.
    /// @return The requested `GeometryConfig`s associated with the component.
    inline std::vector<GeometryConfig> get_geometries() {
        return get_geometries({});
    }

    /// @brief Returns `GeometryConfig`s associated with the calling servo.
    /// @param extra Any additional arguments to the method.
    /// @return The requested `GeometryConfig`s associated with the component.
    virtual std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) = 0;

   protected:
    explicit Servo(std::string name) : Component(std::move(name)){};
};

template <>
struct API::traits<Servo> {
    static API api();
};

}  // namespace sdk
}  // namespace viam
