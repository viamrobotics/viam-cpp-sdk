#include <viam/sdk/resource/resource_type.hpp>

namespace viam {
namespace sdk {

bool operator==(const ResourceType& lhs, const ResourceType& rhs) {
    return lhs.type == rhs.type;
}

std::string ResourceType::to_string() {
    return type;
}

}  // namespace sdk
}  // namespace viam
