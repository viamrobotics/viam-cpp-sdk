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
/// @defgroup Generic Classes related to the `Generic` component.

/// @class GenericSubtype
/// @brief Defines a `ResourceSubtype` for the `Generic` component.
/// @ingroup Generic
class GenericSubtype : public ResourceSubtype {
   public:
    std::shared_ptr<ResourceServer> create_resource_server(
        std::shared_ptr<ResourceManager> manager) override;
    std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                std::shared_ptr<grpc::Channel> chan) override;
    GenericSubtype(const google::protobuf::ServiceDescriptor* service_descriptor)
        : ResourceSubtype(service_descriptor){};
};

/// @class Generic generic.hpp "components/generic/generic.hpp"
/// @brief A `Generic` represents any component that can execute arbitrary commands.
/// @ingroup Generic
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific generic implementations. This class cannot be used on its own.
class Generic : public Component {
   public:
    /// @brief Creates a `ResourceSubtype` for the `Generic` component.
    static std::shared_ptr<ResourceSubtype> resource_subtype();

    /// @brief Creates a `Generic` `Subtype`.
    static Subtype subtype();

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(AttributeMap command) = 0;

    Subtype instance_subtype() const override;

   protected:
    explicit Generic(std::string name) : Component(std::move(name)){};
};

}  // namespace sdk
}  // namespace viam
