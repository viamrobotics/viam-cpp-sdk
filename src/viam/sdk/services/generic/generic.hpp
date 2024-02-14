/// @file generic/generic.hpp
///
/// @brief Defines `GenericService` subtype and service class capable of
/// executing arbitrary commands.
#pragma once

#include <google/protobuf/descriptor.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/services/service.hpp>

namespace viam {
namespace sdk {

/// @class GenericService generic.hpp "services/generic/generic.hpp"
/// @brief A `GenericService` represents any service that can execute arbitrary commands.
/// @ingroup GenericService
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific generic service implementations. This class cannot be used on its own.
class GenericService : public Service {
   public:
    /// @brief Send/receive arbitrary commands to the resource.
    /// @param command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(AttributeMap command) = 0;

    /// @brief Creates a `GenericService` `API`.
    API api() const override;

   protected:
    explicit GenericService(std::string name);
};

template <>
struct API::traits<GenericService> {
    static API api();
};

}  // namespace sdk
}  // namespace viam
