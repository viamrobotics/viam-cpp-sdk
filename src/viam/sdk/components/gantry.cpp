#include <viam/sdk/components/gantry.hpp>

namespace viam {
namespace sdk {

API Gantry::api() const {
    return API::get<Gantry>();
}

API API::traits<Gantry>::api() {
    return {kRDK, kComponent, "gantry"};
}

}  // namespace sdk
}  // namespace viam
