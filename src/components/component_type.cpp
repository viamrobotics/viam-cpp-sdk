#include <components/component_type.hpp>

bool operator==(ComponentType &lhs, ComponentType &rhs) {
  return lhs.name == rhs.name;
}
