/// @file components/private/generic_server.hpp
///
/// @brief Implements a gRPC server for the `GenericComponent`.
#pragma once

#include <viam/api/common/v1/common.grpc.pb.h>
#include <viam/api/component/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/components/generic.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class GenericComponentServer
/// @brief gRPC server implementation of a `GenericComponent`.
/// @ingroup GenericComponent
class GenericComponentServer : public ResourceServer,
                               public viam::component::generic::v1::GenericService::Service {
   public:
    using interface_type = GenericComponent;
    using service_type = component::generic::v1::GenericService;
    explicit GenericComponentServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) noexcept override;
    ::grpc::Status GetGeometries(
        ::grpc::ServerContext* context,
        const ::viam::common::v1::GetGeometriesRequest* request,
        ::viam::common::v1::GetGeometriesResponse* response) noexcept override;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
