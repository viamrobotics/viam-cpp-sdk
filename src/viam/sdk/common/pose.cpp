#include <viam/sdk/common/pose.hpp>

namespace viam {
namespace sdk {

bool operator==(const pose_in_frame& lhs, const pose_in_frame& rhs) {
    return lhs.pose == rhs.pose && lhs.reference_frame == rhs.reference_frame;
}
std::ostream& operator<<(std::ostream& os, const pose_in_frame& v) {
    os << "{ pose: " << v.pose << ",\n"
       << "  reference_frame: " << v.reference_frame << "}";
    return os;
}

}  // namespace sdk
}  // namespace viam
