#include <viam/sdk/tests/test_utils.hpp>

#include <unordered_map>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>
#include <viam/sdk/spatialmath/orientation_types.hpp>

namespace viam {
namespace sdktests {

using namespace viam::sdk;

AttributeMap fake_map() {
    auto proto_ptr = std::make_shared<ProtoType>(std::string("hello"));
    AttributeMap map =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();
    map->insert({{std::string("test"), proto_ptr}});
    return map;
}

std::vector<GeometryConfig> fake_geometries() {
    GeometryConfig sphere_config;
    sphere_config.set_geometry_type(GeometryType::sphere);
    sphere_config.set_coordinates({1, 2, 3});
    sphere_config.set_pose({1, 2, 3});
    struct sphere sphere({1});
    sphere_config.set_geometry_specifics(std::move(sphere));
    sphere_config.set_label("sphere");

    GeometryConfig box_config;
    box_config.set_geometry_type(GeometryType::box);
    box_config.set_coordinates({1, 2, 3});
    box_config.set_pose({1, 2, 3});
    struct box box({1, 2, 3});
    box_config.set_geometry_specifics(std::move(box));
    box_config.set_label("box");

    GeometryConfig point_config;
    point_config.set_geometry_type(GeometryType::point);
    point_config.set_coordinates({1, 2, 3});
    point_config.set_pose({1, 2, 3});
    struct sphere point({0});
    point_config.set_geometry_specifics(std::move(point));
    point_config.set_label("point");

    GeometryConfig capsule_config;
    capsule_config.set_geometry_type(GeometryType::capsule);
    capsule_config.set_coordinates({1, 2, 3});
    capsule_config.set_pose({1, 2, 3});
    struct capsule capsule({2, 4});
    capsule_config.set_geometry_specifics(std::move(capsule));
    capsule_config.set_label("capsule");

    return {std::move(sphere_config),
            std::move(box_config),
            std::move(point_config),
            std::move(capsule_config)};
}

TestServer::TestServer(std::shared_ptr<Server> sdk_server) : sdk_server_(sdk_server){};

TestServer::~TestServer() = default;

std::shared_ptr<grpc::Channel> TestServer::grpc_in_process_channel(
    const grpc::ChannelArguments& args) {
    return sdk_server_->server_->InProcessChannel(args);
}

}  // namespace sdktests
}  // namespace viam
