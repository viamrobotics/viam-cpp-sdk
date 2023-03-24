#pragma once

#include <string>

class ComponentType {
public:
  std::string name;
  friend bool operator==(ComponentType &lhs, ComponentType &rhs);
  ComponentType(std::string name) { name = name; }
  ComponentType() { name = "ComponentBase"; }
};
