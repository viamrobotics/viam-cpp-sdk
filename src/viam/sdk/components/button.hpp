/// @file components/button.hpp
///
/// @brief Defines a `Button` component
#pragma once

#include <string>

#include <viam/sdk/components/component.hpp>

namespace viam {
namespace sdk {

/// @defgroup Button Classes related to the Button component.

/// @class Button button.hpp "components/button.hpp"
/// @brief A `Button` represents a physical or virtual button.
/// @ingroup Button
///
/// This acts as an abstract parent class to be inherited from by any drivers representing
/// specific button implementations. This class cannot be used on its own.
class Button : public Component {
   public:
    /// @brief Push the button.
    inline void push() {
        return push({});
    }

    /// @brief Push the button.
    /// @param extra Any additional arguments to the method.
    virtual void push(const ProtoStruct& extra) = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual ProtoStruct do_command(const ProtoStruct& command) = 0;

    API api() const override;

   protected:
    explicit Button(std::string name);
};

template <>
struct API::traits<Button> {
    static API api();
};

}  // namespace sdk
}  // namespace viam
