#include <memory>

#include <grpcpp/support/status.h>

#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/common/proto_utils.hpp>
#include <viam/sdk/services/motion.hpp>
#include <viam/sdk/services/private/navigation_server.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {
namespace impl {

using namespace service::navigation::v1;

::grpc::Status NavigationServer::GetMode(::grpc::ServerContext*,
                                         const GetModeRequest* request,
                                         GetModeResponse* response) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::GetMode", this, request)([&](auto& helper, auto& nav) {
        response->set_mode(Mode(nav->get_mode(request->name(), helper.getExtra())));
    });
}

::grpc::Status NavigationServer::SetMode(::grpc::ServerContext*,
                                         const SetModeRequest* request,
                                         SetModeResponse*) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::SetMode", this, request)([&](auto& helper, auto& nav) {
        nav->set_mode(request->name(), Navigation::Mode(request->mode()), helper.getExtra());
    });
}

::grpc::Status NavigationServer::GetLocation(::grpc::ServerContext*,
                                             const GetLocationRequest* request,
                                             GetLocationResponse* response) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::GetLocation", this, request)([&](auto& helper, auto& nav) {
        const auto& loc = nav->get_location(request->name(), helper.getExtra());
        *response->mutable_location() = loc.location.to_proto();
        response->set_compass_heading(loc.compass_heading);
    });
}

::grpc::Status NavigationServer::GetWaypoints(::grpc::ServerContext*,
                                              const GetWaypointsRequest* request,
                                              GetWaypointsResponse* response) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::GetWaypoints", this, request)([&](auto& helper, auto& nav) {
        const auto& waypoints = nav->get_waypoints(request->name(), helper.getExtra());
        vecToRepeatedPtr(*waypoints, *response->mutable_waypoints(), auto_from_proto);
    });
}

::grpc::Status NavigationServer::AddWaypoint(::grpc::ServerContext*,
                                             const AddWaypointRequest* request,
                                             AddWaypointResponse*) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::AddWaypoint", this, request)([&](auto& helper, auto& nav) {
        nav->add_waypoint(
            request->name(), geo_point::from_proto(request->location()), helper.getExtra());
    });
}

::grpc::Status NavigationServer::RemoveWaypoint(::grpc::ServerContext*,
                                                const RemoveWaypointRequest* request,
                                                RemoveWaypointResponse*) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::RemoveWaypoint", this, request)([&](auto& helper, auto& nav) {
        nav->remove_waypoint(request->name(), request->id(), helper.getExtra());
    });
}

::grpc::Status NavigationServer::GetObstacles(::grpc::ServerContext*,
                                              const GetObstaclesRequest* request,
                                              GetObstaclesResponse* response) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::GetObstacles", this, request)([&](auto& helper, auto& nav) {
        const auto& obstacles = nav->get_obstacles(request->name(), helper.getExtra());
        vecToRepeatedPtr(*obstacles, *response->mutable_obstacles());
    });
}

::grpc::Status NavigationServer::GetPaths(::grpc::ServerContext*,
                                          const GetPathsRequest* request,
                                          GetPathsResponse* response) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::GetPaths", this, request)([&](auto& helper, auto& nav) {
        const auto& paths = nav->get_paths(request->name(), helper.getExtra());
        vecToRepeatedPtr(*paths, *response->mutable_paths(), auto_from_proto);
    });
}

::grpc::Status NavigationServer::GetProperties(::grpc::ServerContext*,
                                               const GetPropertiesRequest* request,
                                               GetPropertiesResponse* response) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::GetProperties", this, request)([&](auto&, auto& nav) {
        response->set_map_type(
            viam::service::navigation::v1::MapType(nav->get_properties(request->name())));
    });
}

::grpc::Status NavigationServer::DoCommand(
    ::grpc::ServerContext*,
    const ::viam::common::v1::DoCommandRequest* request,
    ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Motion>(
        "NavigationServer::DoCommand", this, request)([&](auto&, auto& motion) {
        const ProtoStruct result = motion->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
