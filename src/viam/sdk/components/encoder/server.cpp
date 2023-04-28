#include <viam/sdk/components/encoder/server.hpp>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/encoder/encoder.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {


::grpc::Status EncoderServer::GetPosition(::grpc::ServerContext* context,
                        const ::viam::component::encoder::v1::GetPositionRequest* request,
                        ::viam::component::encoder::v1::GetPositionResponse* response) {
    
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Encoder::GetPosition] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }
    
    std::shared_ptr<Encoder> encoder = std::dynamic_pointer_cast<Encoder>(rb);
    
    Encoder::position result = encoder->get_position(request->position_type());
    response->set_value(result.value);
    response->set_position_type(result.position_type);

    return ::grpc::Status();
}

::grpc::Status EncoderServer::ResetPosition(::grpc::ServerContext* context,
                        const ::viam::component::encoder::v1::ResetPositionRequest* request,
                        ::viam::component::encoder::v1::ResetPositionResponse* response) {
    
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Encoder::ResetPosition] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }
    
    std::shared_ptr<Encoder> encoder = std::dynamic_pointer_cast<Encoder>(rb);
    
    encoder->reset_position();

    return ::grpc::Status();
}

::grpc::Status EncoderServer::GetProperties(::grpc::ServerContext* context,
                        const ::viam::component::encoder::v1::GetPropertiesRequest* request,
                        ::viam::component::encoder::v1::GetPropertiesResponse* response) {
    
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Encoder::GetProperties] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }
    
    std::shared_ptr<Encoder> encoder = std::dynamic_pointer_cast<Encoder>(rb);
    
    Encoder::properties result = encoder->get_properties();
    response->set_ticks_count_supported(result.ticks_count_supported);
    response->set_angle_degrees_supported(result.angle_degrees_supported);

    return ::grpc::Status();
}

::grpc::Status EncoderServer::DoCommand(::grpc::ServerContext* context,
                        const ::viam::component::encoder::v1::common.v1.DoCommandRequest* request,
                        ::viam::component::encoder::v1::common.v1.DoCommandResponse* response) {
    
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Encoder::DoCommand] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }
    
    std::shared_ptr<Encoder> encoder = std::dynamic_pointer_cast<Encoder>(rb);
    
    AttributeMap result = encoder->do_command(request->TODO());

    return ::grpc::Status();
}


void EncoderServer::register_server(std::shared_ptr<Server> server) {
    viam::component::encoder::v1::EncoderService::Service* encoder =
        static_cast<viam::component::encoder::v1::EncoderService::Service*>(this);
    server->register_service(encoder);
}

std::shared_ptr<SubtypeService> EncoderServer::sub_svc() {
    return sub_svc;
}

}  // namespace sdk
}  // namespace viam
