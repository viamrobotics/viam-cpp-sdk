/// @file services/private/navigation_server.hpp
///
/// @brief Implements a gRPC server for the `Navigation` service.
#pragma once

#include <memory>

#include <viam/api/service/navigation/v1/navigation.grpc.pb.h>

#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/services/navigation.hpp>

namespace viam {
namespace sdk {
namespace impl {

using namespace service::navigation::v1;

/// @class NavigationServer
/// @brief gRPC server implementation of a `Navigation` service.
/// @ingroup Navigation
class NavigationServer : public ResourceServer,
                         public viam::service::navigation::v1::NavigationService::Service {
   public:
    using interface_type = Navigation;
    using service_type = service::navigation::v1::NavigationService;
    explicit NavigationServer(std::shared_ptr<ResourceManager> manager) : ResourceServer(std::move(manager)){}

    ::grpc::Status GetMode(::grpc::ServerContext* context,
                           const GetModeRequest* request,
                           GetModeResponse* response) noexcept override;
    ::grpc::Status SetMode(::grpc::ServerContext* context,
                           const SetModeRequest* request,
                           SetModeResponse* response) noexcept override;
    ::grpc::Status GetLocation(::grpc::ServerContext* context,
                               const GetLocationRequest* request,
                               GetLocationResponse* response) noexcept override;
    ::grpc::Status GetWaypoints(::grpc::ServerContext* context,
                                const GetWaypointsRequest* request,
                                GetWaypointsResponse* response) noexcept override;
    ::grpc::Status AddWaypoint(::grpc::ServerContext* context,
                               const AddWaypointRequest* request,
                               AddWaypointResponse* response) noexcept override;
    ::grpc::Status RemoveWaypoint(::grpc::ServerContext* context,
                                  const RemoveWaypointRequest* request,
                                  RemoveWaypointResponse* response) noexcept override;
    ::grpc::Status GetObstacles(::grpc::ServerContext* context,
                                const GetObstaclesRequest* request,
                                GetObstaclesResponse* response) noexcept override;
    ::grpc::Status GetPaths(::grpc::ServerContext* context,
                            const GetPathsRequest* request,
                            GetPathsResponse* response) noexcept override;
    ::grpc::Status GetProperties(::grpc::ServerContext* context,
                                 const GetPropertiesRequest* request,
                                 GetPropertiesResponse* response) noexcept override;
    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) noexcept override;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
