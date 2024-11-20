/// @file generic.hpp
///
/// @brief Defines `GenericComponent` subtype and component class capable of executing arbitrary
/// commands.
#pragma once

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>

namespace viam {
namespace sdk {

/// @class GenericComponent generic.hpp "components/generic.hpp"
/// @brief A `GenericComponent` represents any component that can execute arbitrary commands.
/// @ingroup GenericComponent
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific generic implementations. This class cannot be used on its own.
class GenericComponent : public Component {
   public:
    /// @brief Send/receive arbitrary commands to the resource.
    /// @param command the command to execute.
    /// @return The result of the executed command.
    virtual ProtoStruct do_command(const ProtoStruct& command) = 0;

    /// @brief Returns `GeometryConfig`s associated with the calling camera.
    /// @return The requested `GeometryConfig`s associated with the component.
    inline std::vector<GeometryConfig> get_geometries() {
        return get_geometries({});
    }

    virtual std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) = 0;

    /// @brief Creates a `GenericComponent` `API`.
    API api() const override;

   protected:
    explicit GenericComponent(std::string name);
};

template <>
struct API::traits<GenericComponent> {
    static API api();
};

}  // namespace sdk
}  // namespace viam
