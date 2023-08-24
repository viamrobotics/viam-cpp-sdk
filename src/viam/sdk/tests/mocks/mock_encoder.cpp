#include <viam/sdk/tests/mocks/mock_encoder.hpp>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/encoder/v1/encoder.grpc.pb.h>
#include <viam/api/component/encoder/v1/encoder.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/encoder/encoder.hpp>
#include <viam/sdk/components/encoder/server.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace encoder {

using namespace viam::sdk;

Encoder::position MockEncoder::get_position(const AttributeMap& extra,
                                            position_type position_type) {
    this->peek_get_position_position_type = position_type;
    return this->peek_get_position_ret;
};

void MockEncoder::reset_position(const AttributeMap& extra) {
    this->peek_reset_position_called = true;
};

Encoder::properties MockEncoder::get_properties(const AttributeMap& extra) {
    return this->peek_get_properties_ret;
};

std::vector<GeometryConfig> MockEncoder::get_geometries(const sdk::AttributeMap& extra) {
    return fake_geometries();
}

AttributeMap MockEncoder::do_command(AttributeMap command) {
    return command;
};

std::shared_ptr<MockEncoder> MockEncoder::get_mock_encoder() {
    return std::make_shared<MockEncoder>("mock_encoder");
}

}  // namespace encoder
}  // namespace sdktests
}  // namespace viam
