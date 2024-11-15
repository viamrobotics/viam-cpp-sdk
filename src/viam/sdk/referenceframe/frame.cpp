#include <viam/sdk/referenceframe/frame.hpp>

#include <string>

#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>
#include <viam/sdk/spatialmath/orientation_types.hpp>

namespace viam {
namespace sdk {

LinkConfig::LinkConfig(translation translation,
                       OrientationConfig ocfg,
                       GeometryConfig gcfg,
                       std::string parent)
    : translation_(std::move(translation)),
      orientation_(std::move(ocfg)),
      geometry_(std::move(gcfg)),
      parent_(std::move(parent)) {}

LinkConfig::LinkConfig() = default;

translation LinkConfig::get_translation() const {
    return translation_;
}

OrientationConfig LinkConfig::get_orientation_config() const {
    return orientation_;
}

GeometryConfig LinkConfig::get_geometry_config() const {
    return geometry_;
}

std::string LinkConfig::get_parent() const {
    return parent_;
}

}  // namespace sdk
}  // namespace viam
