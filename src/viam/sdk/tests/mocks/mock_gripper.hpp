#pragma once

#include <memory>
#include <string>

#include <viam/sdk/components/gripper.hpp>

namespace viam {
namespace sdktests {
namespace gripper {

class MockGripper : public sdk::Gripper {
   public:
    MockGripper(std::string name) : Gripper(std::move(name)) {}

    static std::shared_ptr<MockGripper> get_mock_gripper();

    void open(const sdk::AttributeMap& extra) override;
    bool grab(const sdk::AttributeMap& extra) override;
    void stop(const sdk::AttributeMap& extra) override;
    bool is_moving() override;
    sdk::AttributeMap do_command(const sdk::AttributeMap& command) override;
    std::vector<sdk::GeometryConfig> get_geometries(const sdk::AttributeMap& extra) override;

    bool peek_open_called{false};
    bool peek_stop_called{false};
    sdk::AttributeMap peek_command;
};

}  // namespace gripper
}  // namespace sdktests
}  // namespace viam
