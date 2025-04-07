/// @file components/switch.hpp
///
/// @brief Defines a `Switch` component
#pragma once

#include <cstdint>

#include <string>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdk {

/// @defgroup Switch Classes related to the Switch component.

/// @class Switch switch.hpp "components/switch.hpp"
/// @brief A `Switch` represents a physical switch with multiple positions.
/// @ingroup Switch
///
/// This acts as an abstract parent class to be inherited from by any drivers representing
/// specific switch implementations. This class cannot be used on its own.
class Switch : public Component {
   public:
    /// @brief Set the position of the switch.
    /// @param position The position to set the switch to.
    inline void set_position(uint32_t position) {
        set_position(position, {});
    }

    /// @brief Set the position of the switch.
    /// @param position The position to set the switch to.
    /// @param extra Any additional arguments to the method.
    virtual void set_position(uint32_t position, const ProtoStruct& extra) = 0;

    /// @brief Get the current position of the switch.
    /// @return The current position of the switch.
    inline uint32_t get_position() {
        return get_position({});
    }

    /// @brief Get the current position of the switch.
    /// @param extra Any additional arguments to the method.
    /// @return The current position of the switch.
    virtual uint32_t get_position(const ProtoStruct& extra) = 0;

    /// @brief Get the number of positions that the switch supports.
    /// @return The number of positions that the switch supports.
    inline uint32_t get_number_of_positions() {
        return get_number_of_positions({});
    }

    /// @brief Get the number of positions that the switch supports.
    /// @param extra Any additional arguments to the method.
    /// @return The number of positions that the switch supports.
    virtual uint32_t get_number_of_positions(const ProtoStruct& extra) = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param command The command to execute.
    /// @return The result of the executed command.
    virtual ProtoStruct do_command(const ProtoStruct& command) = 0;

    API api() const override;

   protected:
    explicit Switch(std::string name);
};

template <>
struct API::traits<Switch> {
    static API api();
};

}  // namespace sdk
}  // namespace viam
