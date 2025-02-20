#pragma once

#include <viam/sdk/services/discovery.hpp>

namespace viam {
namespace sdktests {
namespace discovery {

class MockDiscovery : public sdk::Discovery {
   public:
    MockDiscovery(std::string);

    std::vector<sdk::ResourceConfig> discover_resources(const sdk::ProtoStruct&) override;
    sdk::ProtoStruct do_command(const sdk::ProtoStruct& command) override;
};

}  // namespace discovery
}  // namespace sdktests
}  // namespace viam
