#include <viam/sdk/components/encoder/client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/encoder/v1/encoder.grpc.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/encoder/encoder.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {


Encoder::position EncoderClient::get_position(position_type position_type) {
    viam::component::encoder::v1::GetPositionRequest request;
    viam::component::encoder::v1::GetPositionResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_position_type(position_type);

    grpc::Status status = stub_->GetPosition(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
    
}

void EncoderClient::reset_position() {
    viam::component::encoder::v1::ResetPositionRequest request;
    viam::component::encoder::v1::ResetPositionResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();

    grpc::Status status = stub_->ResetPosition(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
}

Encoder::properties EncoderClient::get_properties() {
    viam::component::encoder::v1::GetPropertiesRequest request;
    viam::component::encoder::v1::GetPropertiesResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();

    grpc::Status status = stub_->GetProperties(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
    
}

AttributeMap EncoderClient::do_command(ERROR TODO) {
    viam::component::encoder::v1::common.v1.DoCommandRequest request;
    viam::component::encoder::v1::common.v1.DoCommandResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_TODO(TODO);

    grpc::Status status = stub_->DoCommand(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return from_proto(response);
    
}


}  // namespace sdk
}  // namespace viam
