/// @file generic/generic.hpp
///
/// @brief Defines `Generic` subtype and component class capable of executing arbitrary commands.
#pragma once

#include <google/protobuf/descriptor.h>

#include <common/proto_type.hpp>
#include <common/utils.hpp>
#include <config/resource.hpp>
#include <registry/registry.hpp>
#include <subtype/subtype.hpp>

// TODO(RSDK-1742): one class per header
/// @defgroup Generic Classes related to the `Generic` component.

/// @class GenericSubtype
/// @brief Defines a `ResourceSubtype` for the `Generic` component.
/// @ingroup Generic
class GenericSubtype : public ResourceSubtype {
   public:
    std::shared_ptr<ResourceServerBase> create_resource_server(
        std::shared_ptr<SubtypeService> svc) override;
    std::shared_ptr<ResourceBase> create_rpc_client(std::string name,
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
class Generic : public ComponentBase {
   public:
    /// @brief Creates a `ResourceSubtype` for the `Generic` component.
    static std::shared_ptr<ResourceSubtype> resource_subtype();

    /// @brief Creates a `Generic` `Subtype`.
    static Subtype subtype();

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(AttributeMap command) = 0;

   protected:
    explicit Generic(std::string name) : ComponentBase(std::move(name)){};
};
