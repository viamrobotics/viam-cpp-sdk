#pragma once

#include <subtype/subtype.hpp>
class ResourceServerBase {
 public:
  virtual void register_server();

 private:
  std::shared_ptr<SubtypeService> sub_svc;
};
