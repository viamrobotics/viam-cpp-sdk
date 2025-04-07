#pragma once

#include <viam/sdk/components/switch.hpp>

namespace viam {
namespace sdktests {
namespace switch_ {

class MockSwitch : public sdk::Switch {
   public:
    MockSwitch(std::string name) : Switch(std::move(name)) {}

    static std::shared_ptr<MockSwitch> get_mock_switch();

    void set_position(uint32_t position, const sdk::ProtoStruct&) override;
    uint32_t get_position(const sdk::ProtoStruct&) override;
    uint32_t get_number_of_positions(const sdk::ProtoStruct&) override;
    sdk::ProtoStruct do_command(const sdk::ProtoStruct& command) override;

    uint32_t current_position;
    uint32_t number_of_positions;
    sdk::ProtoStruct peek_command;
};

}  // namespace switch_
}  // namespace sdktests
}  // namespace viam
