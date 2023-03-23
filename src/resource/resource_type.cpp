#include <resource/resource_type.hpp>

bool operator==(const ResourceType& lhs, const ResourceType& rhs) {
    return lhs.type == rhs.type;
}
