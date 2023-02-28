#include <services/service_type.hpp>

bool operator==(ServiceType& lhs, ServiceType& rhs) {
    return lhs.name == rhs.name;
}

