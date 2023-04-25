#include <viam/sdk/resource/resource_type.hpp>

namespace viam {
namespace sdk {

bool operator==(const ResourceType& lhs, const ResourceType& rhs) {
    return lhs.type_ == rhs.type_;
}

const std::string ResourceType::to_string() const {
    return type_;
}

}  // namespace sdk
}  // namespace viam
