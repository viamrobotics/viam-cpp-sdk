/// @file components/gantry.hpp
///
/// @brief Defines a `Gantry` component
#pragma once

#include <viam/sdk/common/kinematics.hpp>
#include <viam/sdk/resource/stoppable.hpp>

namespace viam {
namespace sdk {

/// @defgroup Gantry Classes related to the Gantry component

/// @class Gantry gantry.hpp "components/gantry.hpp"
/// @brief A `Gantry` represents a physical gantry and can be used for controlling gantries of N
/// axes.
/// @ingroup Gantry
///
/// This acts as an abstract parent class to be inherited from by any drivers representing
/// specific gantry implementations. This class cannot be used on its own.
class Gantry : public Component, public Stoppable {
   public:
    /// @struct movement_coordinate
    /// @brief A coordinate for moving a single axis of a gantry to a desired position at a
    /// requested speed.
    struct movement_coordinate {
        /// @brief A position for the axis of the gantry to move to, in millimeters.
        double position_mm;

        /// @brief A speed in millimeters per second for the gantry to move at respective to the
        /// axis.
        double speed_mm_per_sec;
    };

    /// @brief Get the positions of the axes of the gantry in millimeters.
    inline std::vector<double> get_position() {
        return get_position({});
    }

    /// @brief Get the positions of the axes of the gantry in millimeters.
    /// @param extra Any additional arguments to the method
    virtual std::vector<double> get_position(const ProtoStruct& extra) = 0;

    /// @brief Move the axes of the gantry to the desired positions (mm) at the requested speeds
    /// (mm/sec).
    /// @param coordinates A vector whose entries represent the position and speed pairs for each
    /// axis.
    inline void move_to_position(const std::vector<movement_coordinate>& coordinates) {
        move_to_position(coordinates, {});
    }

    /// @brief Move the axes of the gantry to the desired positions (mm) at the requested speeds
    /// (mm/sec).
    /// @param coordinates A vector whose entries represent the position and speed pairs for each
    /// axis.
    /// @param extra Any additional arguments to the method
    virtual void move_to_position(const std::vector<movement_coordinate>& coordinates,
                                  const ProtoStruct& extra) = 0;

    /// @brief Run the homing sequence of the gantry to re-calibrate the axes with respect to the
    /// limit switches.
    /// @return Whether the gantry has run the homing sequence successfully.
    inline bool home() {
        return home({});
    }

    /// @brief Run the homing sequence of the gantry to re-calibrate the axes with respect to the
    /// limit switches.
    /// @param extra Any additional arguments to the method
    /// @return Whether the gantry has run the homing sequence successfully.
    virtual bool home(const ProtoStruct& extra) = 0;

    /// @brief Get the lengths of the axes of the gantry in millimeters.
    inline std::vector<double> get_lengths() {
        return get_lengths({});
    }

    /// @brief Get the lengths of the axes of the gantry in millimeters.
    /// @param extra Any additional arguments to the method
    virtual std::vector<double> get_lengths(const ProtoStruct& extra) = 0;

    /// @brief Reports if the gantry is in motion.
    virtual bool is_moving() = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual ProtoStruct do_command(const ProtoStruct& command) = 0;

    /// @brief Returns `GeometryConfig`s associated with the calling gantry
    inline std::vector<GeometryConfig> get_geometries() {
        return get_geometries({});
    }

    /// @brief Returns `GeometryConfig`s associated with the calling gantry
    /// @param extra Any additional arguments to the method
    virtual std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) = 0;

    /// @brief Get the kinematics data associated with the gantry.
    /// @param extra Any additional arguments to the method.
    /// @return A variant of kinematics data, with bytes field containing the raw bytes of the file
    /// and the object's type indicating the file format.
    virtual ::viam::sdk::KinematicsData get_kinematics(const ProtoStruct& extra) = 0;

    /// @brief Get the kinematics data associated with the gantry.
    /// @return A variant of kinematics data, with bytes field containing the raw bytes of the file
    /// and the object's type indicating the file format.
    inline ::viam::sdk::KinematicsData get_kinematics() {
        return ::viam::sdk::get_kinematics(
            [this](const ProtoStruct& extra) { return get_kinematics(extra); });
    }

    API api() const override;

   protected:
    using Component::Component;
};

template <>
struct API::traits<Gantry> {
    static API api();
};

}  // namespace sdk
}  // namespace viam
