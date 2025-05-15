/// @file components/private/button_client.hpp
///
/// @brief Implements a gRPC client for the `Button` component
#pragma once

#include <memory>

#include <viam/api/component/button/v1/button.grpc.pb.h>

#include <viam/sdk/components/button.hpp>
#include <viam/sdk/rpc/dial.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class ButtonClient
/// @brief gRPC client implementation of a `Button` component
/// @ingroup Button
class ButtonClient : public Button {
   public:
    using interface_type = Button;
    ButtonClient(std::string name, const ViamChannel& channel);

    const ViamChannel& channel() const {
        return *channel_;
    }

    void push(const ProtoStruct& extra) override;
    ProtoStruct do_command(const ProtoStruct& command) override;

    using Button::push;

   private:
    using StubType = viam::component::button::v1::ButtonService::StubInterface;
    std::unique_ptr<StubType> stub_;
    const ViamChannel* channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
