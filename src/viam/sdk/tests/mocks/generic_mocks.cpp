#include <viam/sdk/tests/mocks/generic_mocks.hpp>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/generic.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace generic {

using namespace viam::sdk;

ProtoStruct MockGenericComponent::do_command(const ProtoStruct&) {
    return map_;
}
std::vector<GeometryConfig> MockGenericComponent::get_geometries(const ProtoStruct&) {
    return geometries_;
}
std::shared_ptr<MockGenericComponent> MockGenericComponent::get_mock_generic() {
    auto generic = std::make_shared<MockGenericComponent>("mock_generic");
    generic->map_ = fake_map();
    generic->geometries_ = fake_geometries();

    return generic;
}

ProtoStruct MockGenericService::do_command(const ProtoStruct&) {
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
