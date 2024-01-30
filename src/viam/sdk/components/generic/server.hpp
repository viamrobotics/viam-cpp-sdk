/// @file components/generic/server.hpp
///
/// @brief Implements a gRPC server for the `GenericComponent`.
#pragma once

#include <viam/api/common/v1/common.grpc.pb.h>
#include <viam/api/component/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {

/// @class GenericComponentServer
/// @brief gRPC server implementation of a `GenericComponent`.
/// @ingroup GenericComponent
class GenericComponentServer : public ResourceServer,
                               public viam::component::generic::v1::GenericService::Service {
   public:
    explicit GenericComponentServer(std::shared_ptr<ResourceManager> manager);
    API api() const override;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) noexcept override;
    ::grpc::Status GetGeometries(
        ::grpc::ServerContext* context,
        const ::viam::common::v1::GetGeometriesRequest* request,
        ::viam::common::v1::GetGeometriesResponse* response) noexcept override;
};

}  // namespace sdk
}  // namespace viam
