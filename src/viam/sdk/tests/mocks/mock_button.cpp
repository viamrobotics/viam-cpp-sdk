#include <viam/sdk/tests/mocks/mock_button.hpp>

#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace button {

using namespace viam::sdk;

std::shared_ptr<MockButton> MockButton::get_mock_button() {
    return std::make_shared<MockButton>("mock_button");
}

void MockButton::push(const ProtoStruct&) {
    peek_push_called = true;
}

ProtoStruct MockButton::do_command(const ProtoStruct& command) {
    return (peek_command = command);
}

}  // namespace button
}  // namespace sdktests
}  // namespace viam
