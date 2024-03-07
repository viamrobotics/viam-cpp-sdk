#include <viam/sdk/tests/mocks/generic_mocks.hpp>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/generic.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace generic {

using namespace viam::sdk;

AttributeMap MockGenericComponent::do_command(const AttributeMap&) {
    return map_;
}
std::vector<GeometryConfig> MockGenericComponent::get_geometries(const AttributeMap&) {
    return geometries_;
}
std::shared_ptr<MockGenericComponent> MockGenericComponent::get_mock_generic() {
    auto generic = std::make_shared<MockGenericComponent>("mock_generic");
    generic->map_ = fake_map();
    generic->geometries_ = fake_geometries();

    return generic;
}

std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>
MockGenericService::do_command(const AttributeMap&) {
    return map_;
}
std::shared_ptr<MockGenericService> MockGenericService::get_mock_generic() {
    auto generic = std::make_shared<MockGenericService>("mock_generic");
    generic->map_ = fake_map();

    return generic;
}

}  // namespace generic
}  // namespace sdktests
}  // namespace viam
