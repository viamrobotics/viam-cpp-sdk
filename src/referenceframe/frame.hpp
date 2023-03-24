#pragma once
#include <app/v1/robot.pb.h>

#include <spatialmath/geometry.hpp>
#include <spatialmath/orientation.hpp>
#include <string>

class LinkConfig {
public:
  viam::app::v1::Frame to_proto();
  static LinkConfig from_proto(viam::app::v1::Frame proto);

private:
  std::string id;
  Viam::SDK::translation translation;
  OrientationConfig orientation;
  GeometryConfig geometry;
  std::string parent;
};
