#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/encoder/v1/encoder.grpc.pb.h>
#include <viam/api/component/encoder/v1/encoder.pb.h>

#include <viam/sdk/components/encoder/client.hpp>
#include <viam/sdk/components/encoder/encoder.hpp>
#include <viam/sdk/components/encoder/server.hpp>

namespace viam {
namespace sdktests {
namespace encoder {

class MockEncoder : public viam::sdk::Encoder {
   public:
    Encoder::position get_position(Encoder::position_type position_type) override;
    void reset_position() override;
    Encoder::properties get_properties() override;
    viam::sdk::AttributeMap do_command(viam::sdk::AttributeMap command) override;
    static std::shared_ptr<MockEncoder> get_mock_encoder();

    MockEncoder(std::string name) : Encoder(std::move(name)){};

    // For testing purposes only.
    bool peek_reset_position_called;
    Encoder::properties peek_get_properties_ret;
    Encoder::position_type peek_get_position_position_type;
    Encoder::position peek_get_position_ret;
};

}  // namespace encoder
}  // namespace sdktests
}  // namespace viam
