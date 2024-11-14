#include <viam/sdk/services/navigation.hpp>

#include <viam/api/service/navigation/v1/navigation.pb.h>
#include <viam/sdk/common/proto_utils.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

Navigation::Navigation(std::string name) : Service(std::move(name)){};

API Navigation::api() const {
    return API::get<Navigation>();
}

API API::traits<Navigation>::api() {
    return {kRDK, kService, "navigation"};
}

template <>
Navigation::Waypoint auto_from_proto(const viam::service::navigation::v1::Waypoint& proto) {
    return Navigation::Waypoint{proto.id(), geo_point::from_proto(proto.location())};
}

template <>
viam::service::navigation::v1::Waypoint auto_to_proto(const Navigation::Waypoint& wp) {  // NOLINT
    viam::service::navigation::v1::Waypoint ret;
    *ret.mutable_id() = wp.id;
    *ret.mutable_location() = wp.location.to_proto();
    return ret;
}

template <>
Navigation::Path auto_from_proto(const viam::service::navigation::v1::Path& proto) {
    Navigation::Path ret{proto.destination_waypoint_id()};
    repeatedPtrToVec(proto.geopoints(), ret.geopoints);
    return ret;
}

template <>
viam::service::navigation::v1::Path auto_to_proto(const Navigation::Path& p) {  // NOLINT
    viam::service::navigation::v1::Path ret;
    *ret.mutable_destination_waypoint_id() = p.destination_waypoint_id;
    vecToRepeatedPtr(p.geopoints, *ret.mutable_geopoints());
    return ret;
}

}  // namespace sdk
}  // namespace viam
