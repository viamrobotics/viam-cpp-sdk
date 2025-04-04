#include <viam/sdk/services/private/navigation_client.hpp>

#include <math.h>

#include <grpcpp/support/status.h>

#include <viam/api/service/navigation/v1/navigation.grpc.pb.h>
#include <viam/api/service/navigation/v1/navigation.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/private/repeated_ptr_convert.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/services/navigation.hpp>

namespace viam {
namespace sdk {

namespace proto_convert_details {

template <>
struct from_proto_impl<service::navigation::v1::Path> {
    Navigation::Path operator()(const service::navigation::v1::Path* proto) const {
        return {proto->destination_waypoint_id(), impl::from_repeated_field(proto->geopoints())};
    }
};

template <>
struct from_proto_impl<service::navigation::v1::Waypoint> {
    Navigation::Waypoint operator()(const service::navigation::v1::Waypoint* proto) const {
        return {proto->id(), from_proto(proto->location())};
    }
};

}  // namespace proto_convert_details

namespace impl {

using namespace viam::service::navigation::v1;

NavigationClient::NavigationClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Navigation(std::move(name)),
      stub_(service::navigation::v1::NavigationService::NewStub(channel)),
      channel_(std::move(channel)) {}

Navigation::Mode NavigationClient::get_mode(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetMode)
        .with([&](auto& request) { *request.mutable_extra() = to_proto(extra); })
        .invoke([](auto& response) { return Navigation::Mode(response.mode()); });
}

void NavigationClient::set_mode(const Navigation::Mode mode, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::SetMode)
        .with([&](auto& request) {
            request.set_mode(viam::service::navigation::v1::Mode(mode));
            *request.mutable_extra() = to_proto(extra);
        })
        .invoke([](auto&) {});
}

Navigation::LocationResponse NavigationClient::get_location(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetLocation)
        .with([&](auto& request) { *request.mutable_extra() = to_proto(extra); })
        .invoke([](auto& response) {
            return Navigation::LocationResponse{
                from_proto(response.location()),
                response.compass_heading(),
            };
        });
}

std::vector<Navigation::Waypoint> NavigationClient::get_waypoints(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetWaypoints)
        .with([&](auto& request) { *request.mutable_extra() = to_proto(extra); })
        .invoke([](auto& response) { return impl::from_repeated_field(response.waypoints()); });
}

void NavigationClient::add_waypoint(const geo_point& location, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::AddWaypoint)
        .with([&](auto& request) {
            *request.mutable_location() = to_proto(location);
            *request.mutable_extra() = to_proto(extra);
        })
        .invoke([](auto&) {});
}

void NavigationClient::remove_waypoint(const std::string id, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::RemoveWaypoint)
        .with([&](auto& request) {
            *request.mutable_id() = id;
            *request.mutable_extra() = to_proto(extra);
        })
        .invoke([](auto&) {});
}

std::vector<geo_geometry> NavigationClient::get_obstacles(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetObstacles)
        .with([&](auto& request) { *request.mutable_extra() = to_proto(extra); })
        .invoke([](auto& response) { return impl::from_repeated_field(response.obstacles()); });
}

std::vector<NavigationClient::Path> NavigationClient::get_paths(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetPaths)
        .with([&](auto& request) { *request.mutable_extra() = to_proto(extra); })
        .invoke([](auto& response) { return impl::from_repeated_field(response.paths()); });
}

NavigationClient::Properties NavigationClient::get_properties() {
    return make_client_helper(this, *stub_, &StubType::GetProperties)
        .with([&](auto&) {})
        .invoke([](auto& response) { return Properties{MapType(response.map_type())}; });
}

ProtoStruct NavigationClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
