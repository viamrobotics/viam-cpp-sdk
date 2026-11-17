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

Base::Base(std::string name) : Component(std::move(name)) {}

Base::Base(const ResourceConfig& cfg) : Component(cfg) {}

}  // namespace sdk
}  // namespace viam
