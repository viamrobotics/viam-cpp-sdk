#include <viam/sdk/tests/test_utils.hpp>

#include <unordered_map>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/config/resource.hpp>

namespace viam {
namespace sdktests {

using namespace viam::sdk;

AttributeMap fake_map() {
    std::shared_ptr<ProtoType> proto_ptr =
        std::make_shared<ProtoType>(std::move(std::string("hello")));
    AttributeMap map =
        std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();
    map->insert({{std::string("test"), proto_ptr}});
    return map;
}

}  // namespace sdktests
}  // namespace viam
