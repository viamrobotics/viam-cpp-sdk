#pragma once

#include <memory>
#include <string>

#include <viam/sdk/components/button.hpp>

namespace viam {
namespace sdktests {
namespace button {

class MockButton : public sdk::Button {
   public:
    MockButton(std::string name) : Button(std::move(name)) {}

    static std::shared_ptr<MockButton> get_mock_button();

    void push(const sdk::ProtoStruct& extra) override;
    sdk::ProtoStruct do_command(const sdk::ProtoStruct& command) override;

    bool peek_push_called{false};
    sdk::ProtoStruct peek_command;
};

}  // namespace button
}  // namespace sdktests
}  // namespace viam
