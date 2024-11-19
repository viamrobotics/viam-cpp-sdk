#pragma once

#include <string>

#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/spatialmath/orientation.hpp>

namespace viam {
namespace sdk {

class LinkConfig {
   public:
    LinkConfig();
    LinkConfig(translation translation,
               OrientationConfig ocfg,
               GeometryConfig gcfg,
               std::string parent);
    translation get_translation() const;
    OrientationConfig get_orientation_config() const;
    GeometryConfig get_geometry_config() const;
    std::string get_parent() const;

   private:
    translation translation_;
    OrientationConfig orientation_;
    GeometryConfig geometry_;
    std::string parent_;
};

}  // namespace sdk
}  // namespace viam
