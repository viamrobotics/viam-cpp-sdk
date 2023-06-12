/// @file generic/generic.hpp
///
/// @brief Defines `Generic` subtype and component class capable of executing arbitrary commands.
#pragma once

#include <google/protobuf/descriptor.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>

namespace viam {
namespace sdk {

// TODO(RSDK-1742): one class per header
/// @defgroup Generic Classes related to the Generic component.

/// @class GenericRegistration
/// @brief Defines a `ResourceRegistration` for the `Generic` component.
/// @ingroup Generic
class GenericRegistration : public ResourceRegistration {
   public:
    explicit GenericRegistration(const google::protobuf::ServiceDescriptor* service_descriptor);
    std::shared_ptr<ResourceServer> create_resource_server(
        std::shared_ptr<ResourceManager> manager) override;
    std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                std::shared_ptr<grpc::Channel> chan) override;
};

/// @class Generic generic.hpp "components/generic/generic.hpp"
/// @brief A `Generic` represents any component that can execute arbitrary commands.
/// @ingroup Generic
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific generic implementations. This class cannot be used on its own.
class Generic : public Component {
   public:
    /// @brief Creates a `ResourceRegistration` for the `Generic` component.
    static std::shared_ptr<ResourceRegistration> resource_registration();

    /// @brief Creates a `Generic` `API`.
    static API static_api();

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(AttributeMap command) = 0;

    /// @brief Returns `GeometryConfig`s associated with the calling camera.
    /// @return The requested `GeometryConfig`s associated with the component.
    virtual std::vector<GeometryConfig> get_geometries() = 0;

    API dynamic_api() const override;

   protected:
    explicit Generic(std::string name);
};

}  // namespace sdk
}  // namespace viam
