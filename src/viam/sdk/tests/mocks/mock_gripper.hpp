#pragma once

#include <memory>
#include <string>
#include <vector>

#include <viam/sdk/components/gripper.hpp>
#include <viam/sdk/common/proto_type.hpp>

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
    sdk::ProtoStruct get_status() override;
    std::vector<sdk::GeometryConfig> get_geometries(const sdk::ProtoStruct& extra) override;

    // New method declarations
    void set_percent_open(double percent_open, const sdk::ProtoStruct& extra) override;
    double get_percent_open(const sdk::ProtoStruct& extra) override;
    Gripper::properties get_properties(const sdk::ProtoStruct& extra) override;

    bool peek_open_called{false};
    bool peek_stop_called{false};
    sdk::ProtoStruct peek_command;
    // New peek variables
    double peek_percent_open;
    sdk::ProtoStruct peek_get_percent_open_extra;
    sdk::ProtoStruct peek_properties_extra;
};

}  // namespace gripper
}  // namespace sdktests
}  // namespace viam