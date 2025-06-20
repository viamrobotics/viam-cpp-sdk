/// @file components/gripper.hpp
///
/// @brief Defines a `Gripper` component
#pragma once

#include <string>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/resource/stoppable.hpp>

namespace viam {
namespace sdk {

/// @defgroup Gripper Classes related to the Gripper component.

/// @class Gripper gripper.hpp "components/gripper.hpp"
/// @brief A `Gripper` represents a physical robotic gripper.
/// @ingroup Gripper
///
/// This acts as an abstract parent class to be inherited from by any drivers representing
/// specific gripper implementations. This class cannot be used on its own.
class Gripper : public Component, public Stoppable {
   public:
    /// @struct holding_status
    /// @brief whether the gripper is holding something (along with other contextual info)
    struct holding_status {
        bool is_holding_something;
        ProtoStruct meta;
    };

    /// @brief Open the gripper.
    inline void open() {
        return open({});
    }

    /// @brief Open the gripper.
    /// @param extra Any additional arguments to the method.
    virtual void open(const ProtoStruct& extra) = 0;

    /// @brief Instruct the gripper to grab.
    /// @return bool indicating if the gripper grabbed something.
    inline bool grab() {
        return grab({});
    }

    /// @brief Instruct the gripper to grab.
    /// @param extra Any additional arguments to the method.
    /// @return bool indicating if the gripper grabbed something.
    virtual bool grab(const ProtoStruct& extra) = 0;

    /// @brief Reports whether the gripper is holding onto a object (alongside other information).
    /// @return holding_status (see `holding_status` struct for more info).
    inline holding_status is_holding_something() {
        return holding_status({});
    }

    /// @brief Reports whether the gripper is holding onto a object (alongside other information).
    /// @param extra Any additional arguments to the method.
    /// @return holding_status (see `holding_status` struct for more info).
    virtual holding_status is_holding_something(const ProtoStruct& extra) = 0;

    /// @brief Reports if the gripper is in motion.
    virtual bool is_moving() = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual ProtoStruct do_command(const ProtoStruct& command) = 0;

    /// @brief Returns `GeometryConfig`s associated with the calling arm
    inline std::vector<GeometryConfig> get_geometries() {
        return get_geometries({});
    }

    /// @brief Returns `GeometryConfig`s associated with the calling arm
    /// @param extra Any additional arguments to the method
    virtual std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) = 0;

    API api() const override;

   protected:
    explicit Gripper(std::string name);
};

template <>
struct API::traits<Gripper> {
    static API api();
};

}  // namespace sdk
}  // namespace viam
