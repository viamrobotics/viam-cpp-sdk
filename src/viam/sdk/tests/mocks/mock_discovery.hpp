#pragma once

#include <viam/sdk/services/discovery.hpp>

namespace viam {
namespace sdktests {
namespace discovery {

class MockDiscovery : public sdk::Discovery {
   public:
    MockDiscovery(std::string name) : Discovery(std::move(name)) {}

    static std::shared_ptr<MockDiscovery> get_mock_discovery();

    std::vector<ResourceConfig> discover_resources(const sdk::ProtoStruct&) override;
    sdk::ProtoStruct do_command(const sdk::ProtoStruct& command) override;

    bool peek_stop_called;
    sdk::ProtoStruct peek_command;
};

}  // namespace discovery
}  // namespace sdktests
}  // namespace viam
