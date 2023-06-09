#include <viam/sdk/components/encoder/server.hpp>

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
    ::viam::component::encoder::v1::GetPositionResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Encoder::GetPosition] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Encoder> encoder = std::dynamic_pointer_cast<Encoder>(rb);

    const Encoder::position result =
        encoder->get_position(Encoder::from_proto(request->position_type()));
    response->set_value(result.value);
    response->set_position_type(Encoder::to_proto(result.type));

    return ::grpc::Status();
}

::grpc::Status EncoderServer::ResetPosition(
    ::grpc::ServerContext* context,
    const ::viam::component::encoder::v1::ResetPositionRequest* request,
    ::viam::component::encoder::v1::ResetPositionResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Encoder::ResetPosition] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Encoder> encoder = std::dynamic_pointer_cast<Encoder>(rb);

    encoder->reset_position();

    return ::grpc::Status();
}

::grpc::Status EncoderServer::GetProperties(
    ::grpc::ServerContext* context,
    const ::viam::component::encoder::v1::GetPropertiesRequest* request,
    ::viam::component::encoder::v1::GetPropertiesResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Encoder::GetProperties] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Encoder> encoder = std::dynamic_pointer_cast<Encoder>(rb);

    const Encoder::properties result = encoder->get_properties();
    response->set_ticks_count_supported(result.ticks_count_supported);
    response->set_angle_degrees_supported(result.angle_degrees_supported);

    return ::grpc::Status();
}

::grpc::Status EncoderServer::DoCommand(grpc::ServerContext* context,
                                        const viam::common::v1::DoCommandRequest* request,
                                        viam::common::v1::DoCommandResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Encoder::DoCommand] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Encoder> encoder = std::dynamic_pointer_cast<Encoder>(rb);
    const AttributeMap result = encoder->do_command(struct_to_map(request->command()));

    *response->mutable_result() = map_to_struct(result);

    return ::grpc::Status();
}

void EncoderServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

}  // namespace sdk
}  // namespace viam
