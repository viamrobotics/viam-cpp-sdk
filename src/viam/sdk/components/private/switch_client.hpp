/// @file components/private/switch_client.hpp
///
/// @brief Implements a gRPC client for the `Switch` component
#pragma once

#include <viam/api/component/switch/v1/switch.grpc.pb.h>

#include <viam/sdk/components/switch.hpp>
#include <viam/sdk/rpc/dial.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class SwitchClient
/// @brief gRPC client implementation of a `Switch` component.
/// @ingroup Switch
class SwitchClient : public Switch {
   public:
    using interface_type = Switch;
    SwitchClient(std::string name, const ViamChannel& channel);

    const ViamChannel& channel() const {
        return *channel_;
    }

    void set_position(uint32_t position, const ProtoStruct& extra) override;
    uint32_t get_position(const ProtoStruct& extra) override;
    uint32_t get_number_of_positions(const ProtoStruct& extra) override;
    ProtoStruct do_command(const ProtoStruct& command) override;

    // Using declarations to introduce convenience overloads of interface which do not need to be
    // passed the ProtoStruct parameter.
    using Switch::get_number_of_positions;
    using Switch::get_position;
    using Switch::set_position;

   private:
    using StubType = viam::component::switch_::v1::SwitchService::StubInterface;
    std::unique_ptr<StubType> stub_;
    const ViamChannel* channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
