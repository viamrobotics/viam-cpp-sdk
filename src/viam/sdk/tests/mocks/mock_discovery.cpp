#include <viam/sdk/tests/mocks/mock_discovery.hpp>

#include "mock_discovery.hpp"
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace discovery {

MockDiscovery::MockDiscovery(std::string name) : Discovery(std::move(name)) {}

std::vector<sdk::ResourceConfig> MockDiscovery::discover_resources(const sdk::ProtoStruct&) {
    return fake_discovered_resources();
}

ProtoStruct MockDiscovery::do_command(const sdk::ProtoStruct& command) {
    return ProtoStruct{};
}

}  // namespace discovery
}  // namespace sdktests
}  // namespace viam
