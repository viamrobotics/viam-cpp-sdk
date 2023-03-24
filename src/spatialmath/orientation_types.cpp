#include <app/v1/robot.pb.h>

#include <spatialmath/orientation_types.hpp>

viam::app::v1::Translation Viam::SDK::translation::to_proto() {
  viam::app::v1::Translation t;
  t.set_x(x);
  t.set_y(y);
  t.set_z(z);
  return t;
}
