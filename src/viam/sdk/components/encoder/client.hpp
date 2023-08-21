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
    EncoderClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    position get_position(position_type position_type) override;
    position get_position(const AttributeMap& extra, position_type position_type) override;
    void reset_position() override;
    void reset_position(const AttributeMap& extra) override;
    properties get_properties() override;
    properties get_properties(const AttributeMap& extra) override;
    std::vector<GeometryConfig> get_geometries() override;
    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;
    AttributeMap do_command(AttributeMap command) override;

   private:
    std::unique_ptr<viam::component::encoder::v1::EncoderService::StubInterface> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam
