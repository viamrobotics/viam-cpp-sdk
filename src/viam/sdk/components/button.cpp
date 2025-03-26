#include <viam/sdk/components/button.hpp>

#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

API Button::api() const {
    return API::get<Button>();
}

API API::traits<Button>::api() {
    return {kRDK, kComponent, "button"};
}

Button::Button(std::string name) : Component(std::move(name)) {}

}  // namespace sdk
}  // namespace viam
