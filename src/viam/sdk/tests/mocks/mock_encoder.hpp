#pragma once

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/encoder.hpp>

namespace viam {
namespace sdktests {
namespace encoder {

class MockEncoder : public viam::sdk::Encoder {
   public:
    Encoder::position get_position(const sdk::ProtoStruct& extra,
                                   Encoder::position_type position_type) override;
    void reset_position(const sdk::ProtoStruct& extra) override;
    Encoder::properties get_properties(const sdk::ProtoStruct& extra) override;
    std::vector<sdk::GeometryConfig> get_geometries(const sdk::ProtoStruct& extra) override;
    viam::sdk::ProtoStruct do_command(const viam::sdk::ProtoStruct& command) override;
    static std::shared_ptr<MockEncoder> get_mock_encoder();

    MockEncoder(std::string name) : Encoder(std::move(name)) {};

    // For testing purposes only.
    bool peek_reset_position_called;
    Encoder::properties peek_get_properties_ret;
    Encoder::position_type peek_get_position_position_type;
    Encoder::position peek_get_position_ret;
};

}  // namespace encoder
}  // namespace sdktests
}  // namespace viam
