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

    void open(const sdk::ProtoStruct& extra) override;
    bool grab(const sdk::ProtoStruct& extra) override;
    Gripper::holding_status is_holding_something(const sdk::ProtoStruct& extra) override;
    void stop(const sdk::ProtoStruct& extra) override;
    bool is_moving() override;
    sdk::ProtoStruct do_command(const sdk::ProtoStruct& command) override;
    std::vector<sdk::GeometryConfig> get_geometries(const sdk::ProtoStruct& extra) override;

    bool peek_open_called{false};
    bool peek_stop_called{false};
    sdk::ProtoStruct peek_command;
};

}  // namespace gripper
}  // namespace sdktests
}  // namespace viam
