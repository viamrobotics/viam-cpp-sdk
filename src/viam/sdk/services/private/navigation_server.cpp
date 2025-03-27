#include <grpcpp/support/status.h>

#include <memory>
#include <viam/sdk/common/pose.hpp>
#include <viam/sdk/common/private/repeated_ptr_convert.hpp>
#include <viam/sdk/common/private/service_helper.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/services/motion.hpp>
#include <viam/sdk/services/private/navigation_server.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {

namespace proto_convert_details {

template <>
struct to_proto_impl<Navigation::Path> {
    void operator()(const Navigation::Path& self, service::navigation::v1::Path* proto) const {
        *(proto->mutable_destination_waypoint_id()) = self.destination_waypoint_id;
        *(proto->mutable_geopoints()) = impl::to_repeated_field(self.geopoints);
    }
};

template <>
struct to_proto_impl<Navigation::Waypoint> {
    void operator()(const Navigation::Waypoint& self,
                    service::navigation::v1::Waypoint* proto) const {
        *(proto->mutable_id()) = self.id;
        *(proto->mutable_location()) = to_proto(self.location);
    }
};

}  // namespace proto_convert_details

namespace impl {

using namespace service::navigation::v1;

::grpc::Status NavigationServer::GetMode(::grpc::ServerContext*,
                                         const GetModeRequest* request,
                                         GetModeResponse* response) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::GetMode", this, request)([&](auto& helper, auto& nav) {
        response->set_mode(Mode(nav->get_mode(helper.getExtra())));
    });
}

::grpc::Status NavigationServer::SetMode(::grpc::ServerContext*,
                                         const SetModeRequest* request,
                                         SetModeResponse*) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::SetMode", this, request)([&](auto& helper, auto& nav) {
        nav->set_mode(Navigation::Mode(request->mode()), helper.getExtra());
    });
}

::grpc::Status NavigationServer::GetLocation(::grpc::ServerContext*,
                                             const GetLocationRequest* request,
                                             GetLocationResponse* response) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::GetLocation", this, request)([&](auto& helper, auto& nav) {
        const auto& loc = nav->get_location(helper.getExtra());
        *response->mutable_location() = to_proto(loc.location);
        response->set_compass_heading(loc.compass_heading);
    });
}

::grpc::Status NavigationServer::GetWaypoints(::grpc::ServerContext*,
                                              const GetWaypointsRequest* request,
                                              GetWaypointsResponse* response) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::GetWaypoints", this, request)([&](auto& helper, auto& nav) {
        *(response->mutable_waypoints()) =
            impl::to_repeated_field(nav->get_waypoints(helper.getExtra()));
    });
}

::grpc::Status NavigationServer::AddWaypoint(::grpc::ServerContext*,
                                             const AddWaypointRequest* request,
                                             AddWaypointResponse*) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::AddWaypoint", this, request)([&](auto& helper, auto& nav) {
        nav->add_waypoint(from_proto(request->location()), helper.getExtra());
    });
}

::grpc::Status NavigationServer::RemoveWaypoint(::grpc::ServerContext*,
                                                const RemoveWaypointRequest* request,
                                                RemoveWaypointResponse*) noexcept {
    return make_service_helper<Navigation>("NavigationServer::RemoveWaypoint", this, request)(
        [&](auto& helper, auto& nav) { nav->remove_waypoint(request->id(), helper.getExtra()); });
}

::grpc::Status NavigationServer::GetObstacles(::grpc::ServerContext*,
                                              const GetObstaclesRequest* request,
                                              GetObstaclesResponse* response) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::GetObstacles", this, request)([&](auto& helper, auto& nav) {
        *(response->mutable_obstacles()) =
            impl::to_repeated_field(nav->get_obstacles(helper.getExtra()));
    });
}

::grpc::Status NavigationServer::GetPaths(::grpc::ServerContext*,
                                          const GetPathsRequest* request,
                                          GetPathsResponse* response) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::GetPaths", this, request)([&](auto& helper, auto& nav) {
        *response->mutable_paths() = impl::to_repeated_field(nav->get_paths(helper.getExtra()));
    });
}

::grpc::Status NavigationServer::GetProperties(::grpc::ServerContext*,
                                               const GetPropertiesRequest* request,
                                               GetPropertiesResponse* response) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::GetProperties", this, request)([&](auto&, auto& nav) {
        const Navigation::Properties props = nav->get_properties();
        response->set_map_type(MapType(props.map_type));
    });
}

::grpc::Status NavigationServer::DoCommand(
    ::grpc::ServerContext*,
    const ::viam::common::v1::DoCommandRequest* request,
    ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Navigation>(
        "NavigationServer::DoCommand", this, request)([&](auto&, auto& motion) {
        const ProtoStruct result = motion->do_command(from_proto(request->command()));
        *response->mutable_result() = to_proto(result);
    });
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
