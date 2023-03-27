#include <common/proto_type.hpp>
#include <config/resource.hpp>
#include <unordered_map>

AttributeMap fake_map() {
  std::shared_ptr<ProtoType> proto_ptr =
      std::make_shared<ProtoType>(std::move(std::string("hello")));
  AttributeMap map = std::make_shared<
      std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();
  map->insert({{std::string("test"), proto_ptr}});
  return map;
}
