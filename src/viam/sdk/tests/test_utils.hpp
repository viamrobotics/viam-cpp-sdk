#pragma once

#include <unordered_map>

#include <viam/sdk/config/resource.hpp>

namespace viam {
namespace sdktests {

using namespace viam::sdk;

AttributeMap fake_map();
std::vector<GeometryConfig> fake_geometries();

}  // namespace sdktests
}  // namespace viam
