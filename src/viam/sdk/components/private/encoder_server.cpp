#include <viam/sdk/components/private/encoder_server.hpp>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/encoder.hpp>
#include <viam/sdk/components/private/encoder.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {
namespace impl {

EncoderServer::EncoderServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status EncoderServer::GetPosition(
    ::grpc::ServerContext*,
    const ::viam::component::encoder::v1::GetPositionRequest* request,
    ::viam::component::encoder::v1::GetPositionResponse* response) noexcept {
    return make_service_helper<Encoder>(
        "EncoderServer::GetPosition", this, request)([&](auto& helper, auto& encoder) {
        const Encoder::position result =
            encoder->get_position(helper.getExtra(), from_proto(request->position_type()));
        response->set_value(result.value);
        response->set_position_type(to_proto(result.type));
    });
}

::grpc::Status EncoderServer::ResetPosition(
    ::grpc::ServerContext*,
    const ::viam::component::encoder::v1::ResetPositionRequest* request,
    ::viam::component::encoder::v1::ResetPositionResponse*) noexcept {
    return make_service_helper<Encoder>("EncoderServer::ResetPosition", this, request)(
        [&](auto& helper, auto& encoder) { encoder->reset_position(helper.getExtra()); });
}

::grpc::Status EncoderServer::GetProperties(
    ::grpc::ServerContext*,
    const ::viam::component::encoder::v1::GetPropertiesRequest* request,
    ::viam::component::encoder::v1::GetPropertiesResponse* response) noexcept {
    return make_service_helper<Encoder>(
        "EncoderServer::GetProperties", this, request)([&](auto& helper, auto& encoder) {
        const Encoder::properties result = encoder->get_properties(helper.getExtra());
        response->set_ticks_count_supported(result.ticks_count_supported);
        response->set_angle_degrees_supported(result.angle_degrees_supported);
    });
}

::grpc::Status EncoderServer::GetGeometries(
    ::grpc::ServerContext*,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<Encoder>(
        "EncoderServer::GetGeometries", this, request)([&](auto& helper, auto& encoder) {
        const std::vector<GeometryConfig> geometries = encoder->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = v2::to_proto(geometry);
        }
    });
}

::grpc::Status EncoderServer::DoCommand(grpc::ServerContext*,
                                        const viam::common::v1::DoCommandRequest* request,
                                        viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Encoder>(
        "EncoderServer::DoCommand", this, request)([&](auto&, auto& encoder) {
        const ProtoStruct result = encoder->do_command(v2::from_proto(request->command()));
        *response->mutable_result() = v2::to_proto(result);
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
