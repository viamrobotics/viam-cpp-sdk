/// @file generic/generic.hpp
///
/// @brief Defines `GenericComponent` subtype and component class capable of executing arbitrary
/// commands.
#pragma once

#include <google/protobuf/descriptor.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>

namespace viam {
namespace sdk {

// TODO(RSDK-3030): one class per header
/// @defgroup GenericComponent Classes related to the generic component.

/// @class GenericComponentRegistration
/// @brief Defines a `ResourceRegistration` for the generic component.
/// @ingroup GenericComponent
class GenericComponentRegistration : public ResourceRegistration {
   public:
    explicit GenericComponentRegistration(
        const google::protobuf::ServiceDescriptor* service_descriptor);
    std::shared_ptr<ResourceServer> create_resource_server(std::shared_ptr<ResourceManager> manager,
                                                           Server& server) override;
    std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                std::shared_ptr<grpc::Channel> chan) override;
};

/// @class GenericComponent generic.hpp "components/generic/generic.hpp"
/// @brief A `GenericComponent` represents any component that can execute arbitrary commands.
/// @ingroup GenericComponent
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific generic implementations. This class cannot be used on its own.
class GenericComponent : public Component {
   public:
    /// @brief Creates a `ResourceRegistration` for `GenericComponent`.
    static std::shared_ptr<ResourceRegistration> resource_registration();

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(AttributeMap command) = 0;

    /// @brief Returns `GeometryConfig`s associated with the calling camera.
    /// @return The requested `GeometryConfig`s associated with the component.
    inline std::vector<GeometryConfig> get_geometries() {
        return get_geometries({});
    }

    virtual std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) = 0;

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
