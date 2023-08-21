#include <viam/sdk/components/motor/server.hpp>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/motor/motor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

MotorServer::MotorServer() : ResourceServer(std::make_shared<ResourceManager>()){};
MotorServer::MotorServer(std::shared_ptr<ResourceManager> manager) : ResourceServer(manager){};

::grpc::Status MotorServer::SetPower(::grpc::ServerContext* context,
                                     const ::viam::component::motor::v1::SetPowerRequest* request,
                                     ::viam::component::motor::v1::SetPowerResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::SetPower] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    motor->set_power(request->power_pct(), extra);

    return ::grpc::Status();
}

::grpc::Status MotorServer::GoFor(::grpc::ServerContext* context,
                                  const ::viam::component::motor::v1::GoForRequest* request,
                                  ::viam::component::motor::v1::GoForResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::GoFor] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    motor->go_for(request->rpm(), request->revolutions(), extra);

    return ::grpc::Status();
}

::grpc::Status MotorServer::GoTo(::grpc::ServerContext* context,
                                 const ::viam::component::motor::v1::GoToRequest* request,
                                 ::viam::component::motor::v1::GoToResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::GoTo] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    motor->go_to(request->rpm(), request->position_revolutions(), extra);

    return ::grpc::Status();
}

::grpc::Status MotorServer::ResetZeroPosition(
    ::grpc::ServerContext* context,
    const ::viam::component::motor::v1::ResetZeroPositionRequest* request,
    ::viam::component::motor::v1::ResetZeroPositionResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::ResetZeroPosition] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    motor->reset_zero_position(request->offset(), extra);

    return ::grpc::Status();
}

::grpc::Status MotorServer::GetPosition(
    ::grpc::ServerContext* context,
    const ::viam::component::motor::v1::GetPositionRequest* request,
    ::viam::component::motor::v1::GetPositionResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::GetPosition] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    const Motor::position result = motor->get_position(extra);
    response->set_position(result);

    return ::grpc::Status();
}

::grpc::Status MotorServer::GetProperties(
    ::grpc::ServerContext* context,
    const ::viam::component::motor::v1::GetPropertiesRequest* request,
    ::viam::component::motor::v1::GetPropertiesResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::GetProperties] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    const Motor::properties result = motor->get_properties(extra);
    response->set_position_reporting(result.position_reporting);

    return ::grpc::Status();
}

::grpc::Status MotorServer::Stop(::grpc::ServerContext* context,
                                 const ::viam::component::motor::v1::StopRequest* request,
                                 ::viam::component::motor::v1::StopResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::Stop] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    motor->stop(extra);

    return ::grpc::Status();
}

::grpc::Status MotorServer::IsPowered(::grpc::ServerContext* context,
                                      const ::viam::component::motor::v1::IsPoweredRequest* request,
                                      ::viam::component::motor::v1::IsPoweredResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::IsPowered] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    const Motor::power_status result = motor->get_power_status(extra);
    response->set_is_on(result.is_on);
    response->set_power_pct(result.power_pct);

    return ::grpc::Status();
}

::grpc::Status MotorServer::IsMoving(::grpc::ServerContext* context,
                                     const ::viam::component::motor::v1::IsMovingRequest* request,
                                     ::viam::component::motor::v1::IsMovingResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::IsMoving] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);

    const bool result = motor->is_moving();
    response->set_is_moving(result);

    return ::grpc::Status();
}

::grpc::Status MotorServer::GetGeometries(::grpc::ServerContext* context,
                                          const ::viam::common::v1::GetGeometriesRequest* request,
                                          ::viam::common::v1::GetGeometriesResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [GetGeometries] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    AttributeMap extra;
    if (request->has_extra()) {
        extra = struct_to_map(request->extra());
    }

    const std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    const std::vector<GeometryConfig> geometries = motor->get_geometries(extra);
    for (const auto& geometry : geometries) {
        *response->mutable_geometries()->Add() = geometry.to_proto();
    }

    return ::grpc::Status();
}

::grpc::Status MotorServer::DoCommand(grpc::ServerContext* context,
                                      const viam::common::v1::DoCommandRequest* request,
                                      viam::common::v1::DoCommandResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::DoCommand] without a request");
    };

    const std::shared_ptr<Resource> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    const AttributeMap result = motor->do_command(struct_to_map(request->command()));

    *response->mutable_result() = map_to_struct(result);

    return ::grpc::Status();
}

void MotorServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

}  // namespace sdk
}  // namespace viam
