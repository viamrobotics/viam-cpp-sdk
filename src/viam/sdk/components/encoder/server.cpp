#include <viam/sdk/components/encoder/server.hpp>

#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/encoder/encoder.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

EncoderServer::EncoderServer() : ResourceServer(std::make_shared<ResourceManager>()){};
EncoderServer::EncoderServer(std::shared_ptr<ResourceManager> manager) : ResourceServer(manager){};

::grpc::Status EncoderServer::GetPosition(
    ::grpc::ServerContext* context,
    const ::viam::component::encoder::v1::GetPositionRequest* request,
    ::viam::component::encoder::v1::GetPositionResponse* response) noexcept {
    return make_service_helper<Encoder>(
        "EncoderServer::GetPosition", this, request)([&](auto& wrapper, auto& encoder) {
        const Encoder::position result = encoder->get_position(
            wrapper.getExtra(), Encoder::from_proto(request->position_type()));
        response->set_value(result.value);
        response->set_position_type(Encoder::to_proto(result.type));
    });
}

::grpc::Status EncoderServer::ResetPosition(
    ::grpc::ServerContext* context,
    const ::viam::component::encoder::v1::ResetPositionRequest* request,
    ::viam::component::encoder::v1::ResetPositionResponse* response) noexcept {
    return make_service_helper<Encoder>("EncoderServer::GetPosition", this, request)(
        [&](auto& wrapper, auto& encoder) { encoder->reset_position(wrapper.getExtra()); });
}

::grpc::Status EncoderServer::GetProperties(
    ::grpc::ServerContext* context,
    const ::viam::component::encoder::v1::GetPropertiesRequest* request,
    ::viam::component::encoder::v1::GetPropertiesResponse* response) noexcept {
    return make_service_helper<Encoder>(
        "EncoderServer::GetProperties", this, request)([&](auto& wrapper, auto& encoder) {
        const Encoder::properties result = encoder->get_properties(wrapper.getExtra());
        response->set_ticks_count_supported(result.ticks_count_supported);
        response->set_angle_degrees_supported(result.angle_degrees_supported);
    });
}

::grpc::Status EncoderServer::GetGeometries(::grpc::ServerContext* context,
                                            const ::viam::common::v1::GetGeometriesRequest* request,
                                            ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<Encoder>(
        "EncoderServer::GetGeometries", this, request)([&](auto& wrapper, auto& encoder) {
        const std::vector<GeometryConfig> geometries = encoder->get_geometries(wrapper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = geometry.to_proto();
        }
    });
}

::grpc::Status EncoderServer::DoCommand(grpc::ServerContext* context,
                                        const viam::common::v1::DoCommandRequest* request,
                                        viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Encoder>(
        "EncoderServer::DoCommand", this, request)([&](auto& wrapper, auto& encoder) {
        const AttributeMap result = encoder->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
}

void EncoderServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

}  // namespace sdk
}  // namespace viam
