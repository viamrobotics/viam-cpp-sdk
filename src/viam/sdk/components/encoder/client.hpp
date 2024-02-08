/// @file components/encoder/client.hpp
///
/// @brief Implements a gRPC client for the `Encoder` component.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/encoder/v1/encoder.grpc.pb.h>

#include <viam/sdk/components/encoder/encoder.hpp>
#include <viam/sdk/components/encoder/server.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

/// @class EncoderClient
/// @brief gRPC client implementation of a `Encoder` component.
/// @ingroup Encoder
class EncoderClient : public Encoder {
   public:
    using interface_type = Encoder;
    EncoderClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    position get_position(const AttributeMap& extra, position_type position_type) override;
    void reset_position(const AttributeMap& extra) override;
    properties get_properties(const AttributeMap& extra) override;
    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;
    AttributeMap do_command(AttributeMap command) override;

    // the `extra` param is frequently unnecessary but needs to be supported. Ideally, we'd
    // like to live in a world where implementers of derived classes don't need to go out of
    // their way to support two versions of a method (an `extra` version and a non-`extra`
    // version), and users don't need to pass an unnecessary parameters to all method calls.
    //
    // To do this, we define in the parent resource class a non-virtual version of the methods
    // that calls the virtual method and passes a `nullptr` by default in place of the `extra`
    // param. In order to access these versions of the methods within the client code, however,
    // we need to include these `using` lines.
    using Encoder::get_geometries;
    using Encoder::get_position;
    using Encoder::get_properties;
    using Encoder::reset_position;

   private:
    using StubType = viam::component::encoder::v1::EncoderService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam
