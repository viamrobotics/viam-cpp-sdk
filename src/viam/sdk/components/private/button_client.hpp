/// @file components/private/button_client.hpp
///
/// @brief Implements a gRPC client for the `Button` component
#pragma once

#include <memory>

#include <grpcpp/channel.h>

#include <viam/api/component/button/v1/button.grpc.pb.h>

#include <viam/sdk/components/button.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class ButtonClient
/// @brief gRPC client implementation of a `Button` component
/// @ingroup Button
class ButtonClient : public Button {
   public:
    using interface_type = Button;
    ButtonClient(std::string name, std::shared_ptr<grpc::Channel> channel);

    void push(const ProtoStruct& extra) override;
    ProtoStruct do_command(const ProtoStruct& command) override;

    using Button::push;

   private:
    using StubType = viam::component::button::v1::ButtonService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
