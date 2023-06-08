#include <viam/sdk/tests/test_utils.hpp>

#include <unordered_map>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/config/resource.hpp>
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
    OrientationConfig fake_orientation(
        OrientationType::AxisAngles, {1, 2, 3}, quaternion{0, 0, 0, 0});
    GeometryConfig sphere_config;
    sphere_config.set_geometry_type(GeometryType::sphere);
    sphere_config.set_coordinates({1, 2, 3});
    sphere_config.set_pose({1, 2, 3});
    sphere_config.set_radius(1);

    GeometryConfig box_config;
    box_config.set_geometry_type(GeometryType::box);
    box_config.set_coordinates({1, 2, 3});
    box_config.set_pose({1, 2, 3});
    box_config.set_radius(0);

    GeometryConfig point_config;
    box_config.set_geometry_type(GeometryType::point);
    box_config.set_coordinates({1, 2, 3});
    box_config.set_pose({1, 2, 3});
    box_config.set_radius(0);
    box_config.set_orientation_config(fake_orientation);
    std::vector<GeometryConfig> geometries = {sphere_config, box_config, point_config};
    return geometries;
}

}  // namespace sdktests
}  // namespace viam
