#include <tests/test_utils.hpp>

#include <unordered_map>

#include <common/proto_type.hpp>
#include <config/resource.hpp>

using namespace viam::cppsdk;

AttributeMap fake_map() {
    std::shared_ptr<ProtoType> proto_ptr =
        std::make_shared<ProtoType>(std::move(std::string("hello")));
    AttributeMap map =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();
    map->insert({{std::string("test"), proto_ptr}});
    return map;
}
