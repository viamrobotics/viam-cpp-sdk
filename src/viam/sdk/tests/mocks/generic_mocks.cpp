#include <viam/sdk/tests/mocks/generic_mocks.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/camera/v1/camera.grpc.pb.h>
#include <viam/api/component/camera/v1/camera.pb.h>

#include <viam/sdk/components/generic/client.hpp>
#include <viam/sdk/components/generic/generic.hpp>
#include <viam/sdk/components/generic/server.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace generic {

using namespace viam::sdk;

std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>
MockGenericComponent::do_command(
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> command) {
    return map_;
}
std::vector<GeometryConfig> MockGenericComponent::get_geometries(const AttributeMap& extra) {
    return geometries_;
}
std::shared_ptr<MockGenericComponent> MockGenericComponent::get_mock_generic() {
    auto generic = std::make_shared<MockGenericComponent>("mock_generic");
    generic->map_ = fake_map();
    generic->geometries_ = fake_geometries();

    return generic;
}

std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>
MockGenericService::do_command(
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> command) {
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
