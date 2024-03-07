/// @file services/generic/server.hpp
///
/// @brief Implements a gRPC server for the `GenericService`.
#pragma once

#include <viam/api/common/v1/common.grpc.pb.h>
#include <viam/api/service/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>
#include <viam/sdk/services/generic.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class GenericServiceServer
/// @brief gRPC server implementation of a `GenericService`.
/// @ingroup GenericService
class GenericServiceServer : public ResourceServer,
                             public viam::service::generic::v1::GenericService::Service {
   public:
    using interface_type = GenericService;
    using service_type = service::generic::v1::GenericService;
    explicit GenericServiceServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) noexcept override;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
