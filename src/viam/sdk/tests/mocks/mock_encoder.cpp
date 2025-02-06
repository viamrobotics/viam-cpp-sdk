#include <viam/sdk/tests/mocks/mock_encoder.hpp>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/encoder.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace encoder {

using namespace viam::sdk;

Encoder::position MockEncoder::get_position(const ProtoStruct&, position_type position_type) {
    this->peek_get_position_position_type = position_type;
    return this->peek_get_position_ret;
};

void MockEncoder::reset_position(const ProtoStruct&) {
    this->peek_reset_position_called = true;
};

Encoder::properties MockEncoder::get_properties(const ProtoStruct&) {
    return this->peek_get_properties_ret;
};

std::vector<GeometryConfig> MockEncoder::get_geometries(const sdk::ProtoStruct&) {
    return fake_geometries();
}

ProtoStruct MockEncoder::do_command(const ProtoStruct& command) {
    return command;
};

std::shared_ptr<MockEncoder> MockEncoder::get_mock_encoder() {
    return std::make_shared<MockEncoder>("mock_encoder");
}

}  // namespace encoder
}  // namespace sdktests
}  // namespace viam
