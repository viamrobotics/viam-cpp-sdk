#pragma once

#include <string>

class ServiceType {
public:
  std::string name;
  friend bool operator==(ServiceType &lhs, ServiceType &rhs);
  ServiceType(std::string name) { name = name; }
  ServiceType() { name = "ServiceBase"; }
};
