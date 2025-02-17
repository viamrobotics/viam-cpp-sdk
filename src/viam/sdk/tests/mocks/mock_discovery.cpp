#include <viam/sdk/tests/mocks/mock_discovery.hpp>

#include "mock_discovery.hpp"
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace discovery {

std::shared_ptr<MockDiscovery> MockDiscovery::get_mock_discovery() {
    return std::make_shared<MockDiscovery>("mock_discovery");
}

std::vector<sdk::ResourceConfig> MockDiscovery::discover_resources(const sdk::ProtoStruct&) {
    return fake_resources();
}

sdk::ProtoStruct MockDiscovery::do_command(const sdk::ProtoStruct& command) {
    return (peek_command = command);
}


} // namespace discovery
} // namespace sdktests
} // namespace viam
