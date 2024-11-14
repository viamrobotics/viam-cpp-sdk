#include <viam/sdk/services/private/navigation_client.hpp>

#include <math.h>

#include <grpcpp/support/status.h>

#include <viam/api/service/navigation/v1/navigation.grpc.pb.h>
#include <viam/api/service/navigation/v1/navigation.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/proto_utils.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/services/navigation.hpp>

namespace viam {
namespace sdk {
namespace impl {

using namespace viam::service::navigation::v1;

NavigationClient::NavigationClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Navigation(std::move(name)),
      stub_(service::navigation::v1::NavigationService::NewStub(channel)),
      channel_(std::move(channel)){};

Navigation::Mode NavigationClient::get_mode(const std::string name, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetMode)
        .with([&](auto& request) {
            *request.mutable_name() = name;
            *request.mutable_extra() = map_to_struct(extra);
        })
        .invoke([](auto& response) { return Navigation::Mode(response.mode()); });
}

void NavigationClient::set_mode(const std::string name,
                                const Navigation::Mode mode,
                                const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::SetMode)
        .with([&](auto& request) {
            *request.mutable_name() = name;
            request.set_mode(viam::service::navigation::v1::Mode(mode));
            *request.mutable_extra() = map_to_struct(extra);
        })
        .invoke([](auto& response) {});
}

Navigation::LocationResponse NavigationClient::get_location(const std::string name,
                                                            const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetLocation)
        .with([&](auto& request) {
            *request.mutable_name() = name;
            *request.mutable_extra() = map_to_struct(extra);
        })
        .invoke([](auto& response) {
            return Navigation::LocationResponse{
                geo_point::from_proto(response.location()),
                response.compass_heading(),
            };
        });
}

std::unique_ptr<std::vector<Navigation::Waypoint>> NavigationClient::get_waypoints(
    const std::string name, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetWaypoints)
        .with([&](auto& request) {
            *request.mutable_name() = name;
            *request.mutable_extra() = map_to_struct(extra);
        })
        .invoke([](auto& response) {
            auto ret = std::make_unique<std::vector<Navigation::Waypoint>>();
            repeatedPtrToVec(response.waypoints(), *ret, auto_to_proto);
            return ret;
        });
}

void NavigationClient::add_waypoint(const std::string name,
                                    const geo_point& location,
                                    const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::AddWaypoint)
        .with([&](auto& request) {
            *request.mutable_name() = name;
            *request.mutable_location() = location.to_proto();
            *request.mutable_extra() = map_to_struct(extra);
        })
        .invoke([](auto& response) {});
}

void NavigationClient::remove_waypoint(const std::string name,
                                       const std::string id,
                                       const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::RemoveWaypoint)
        .with([&](auto& request) {
            *request.mutable_name() = name;
            *request.mutable_id() = id;
            *request.mutable_extra() = map_to_struct(extra);
        })
        .invoke([](auto& response) {});
}

std::unique_ptr<std::vector<geo_geometry>> NavigationClient::get_obstacles(
    const std::string name, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetObstacles)
        .with([&](auto& request) {
            *request.mutable_name() = name;
            *request.mutable_extra() = map_to_struct(extra);
        })
        .invoke([](auto& response) {
            auto ret = std::make_unique<std::vector<geo_geometry>>();
            repeatedPtrToVec(response.obstacles(), *ret);
            return ret;
        });
}

std::unique_ptr<std::vector<NavigationClient::Path>> NavigationClient::get_paths(
    const std::string name, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetPaths)
        .with([&](auto& request) {
            *request.mutable_name() = name;
            *request.mutable_extra() = map_to_struct(extra);
        })
        .invoke([](auto& response) {
            auto ret = std::make_unique<std::vector<Path>>();
            repeatedPtrToVec(response.paths(), *ret, auto_to_proto);
            return ret;
        });
}

NavigationClient::MapType NavigationClient::get_properties(const std::string name) {
    return make_client_helper(this, *stub_, &StubType::GetProperties)
        .with([&](auto& request) { *request.mutable_name() = name; })
        .invoke([](auto& response) { return MapType(response.map_type()); });
}

ProtoStruct NavigationClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam