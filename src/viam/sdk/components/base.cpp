#include <viam/sdk/components/base.hpp>

namespace viam {
namespace sdk {

API Base::api() const {
    return API::get<Base>();
}

API API::traits<Base>::api() {
    return {kRDK, kComponent, "base"};
}

std::ostream& operator<<(std::ostream& os, const Base::properties& v) {
    os << "{ turning_radius_meters: " << v.turning_radius_meters
       << ", wheel_circumference_meters: " << v.wheel_circumference_meters
       << ", width_meters: " << v.width_meters << " }";
    return os;
}

bool operator==(const Base::properties& lhs, const Base::properties& rhs) {
    return lhs.wheel_circumference_meters == rhs.wheel_circumference_meters &&
           lhs.width_meters == rhs.width_meters &&
           lhs.turning_radius_meters == rhs.turning_radius_meters;
}

Base::Base(std::string name) : Component(std::move(name)) {};

}  // namespace sdk
}  // namespace viam
