/// @file generic/generic.hpp
///
/// @brief Defines `GenericService` subtype and service class capable of
/// executing arbitrary commands.
#pragma once

#include <google/protobuf/descriptor.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/services/service.hpp>

namespace viam {
namespace sdk {

// TODO(RSDK-3030): one class per header
/// @defgroup GenericService Classes related to the generic service.

/// @class GenericServiceRegistration
/// @brief Defines a `ResourceRegistration` for the `GenericService`.
/// @ingroup GenericService
class GenericServiceRegistration : public ResourceRegistration {
   public:
    explicit GenericServiceRegistration(
        const google::protobuf::ServiceDescriptor* service_descriptor);
    std::shared_ptr<ResourceServer> create_resource_server(std::shared_ptr<ResourceManager> manager,
                                                           Server& server) override;
    std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                std::shared_ptr<grpc::Channel> chan) override;
};

/// @class GenericService generic.hpp "services/generic/generic.hpp"
/// @brief A `GenericService` represents any service that can execute arbitrary commands.
/// @ingroup GenericService
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific generic service implementations. This class cannot be used on its own.
class GenericService : public Service {
   public:
    /// @brief Creates a `ResourceRegistration` for the `GenericService`.
    static std::shared_ptr<ResourceRegistration> resource_registration();

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
