#include <viam/sdk/services/private/navigation_client.hpp>

#include <math.h>

#include <grpcpp/support/status.h>

#include <viam/api/service/navigation/v1/navigation.grpc.pb.h>
#include <viam/api/service/navigation/v1/navigation.pb.h>

#include <viam/sdk/common/client_helper.hpp>
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
                response.location(),
                response.compass_heading(),
            };
        });
}

// helper; unpacks a repeated pointer to a vector of destination type.
// Dst must have a constructor that takes Src*.
template <typename Src, typename Dst>
std::unique_ptr<std::vector<Dst>> repeatedPtrToVector(
    const ::google::protobuf::RepeatedPtrField<Src>& items) {
    auto vec = std::make_unique<std::vector<Dst>>();
    vec->reserve(items.size());
    for (auto& x : items) {
        vec->push_back(x);
    }
    return vec;
}

std::unique_ptr<std::vector<Navigation::Waypoint>> NavigationClient::get_waypoints(
    const std::string name, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetWaypoints)
        .with([&](auto& request) {
            *request.mutable_name() = name;
            *request.mutable_extra() = map_to_struct(extra);
        })
        .invoke([](auto& response) {
            return repeatedPtrToVector<viam::service::navigation::v1::Waypoint,
                                       Navigation::Waypoint>(response.waypoints());
        });
}

void NavigationClient::add_waypoint(const std::string name,
                                    const GeoPoint& location,
                                    const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::AddWaypoint)
        .with([&](auto& request) {
            *request.mutable_name() = name;
            *request.mutable_location() = location;
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

std::unique_ptr<std::vector<GeoGeometry>> NavigationClient::get_obstacles(
    const std::string name, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetObstacles)
        .with([&](auto& request) {
            *request.mutable_name() = name;
            *request.mutable_extra() = map_to_struct(extra);
        })
        .invoke([](auto& response) {
            return std::make_unique<std::vector<GeoGeometry>>(response.obstacles().begin(),
                                                              response.obstacles().end());
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
            return std::make_unique<std::vector<Path>>(response.paths().begin(),
                                                       response.paths().end());
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
