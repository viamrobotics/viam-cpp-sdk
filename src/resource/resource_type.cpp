#include <resource/resource_type.hpp>

using namespace viam::cppsdk;

bool operator==(const ResourceType& lhs, const ResourceType& rhs) {
    return lhs.type == rhs.type;
}

std::string ResourceType::to_string() {
    return type;
}
