#include <viam/sdk/components/motor/server.hpp>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/motor/motor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

::grpc::Status MotorServer::SetPower(::grpc::ServerContext* context,
                                     const ::viam::component::motor::v1::SetPowerRequest* request,
                                     ::viam::component::motor::v1::SetPowerResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::SetPower] without a request");
    };

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);

    motor->set_power(request->power_pct());

    return ::grpc::Status();
}

::grpc::Status MotorServer::GoFor(::grpc::ServerContext* context,
                                  const ::viam::component::motor::v1::GoForRequest* request,
                                  ::viam::component::motor::v1::GoForResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::GoFor] without a request");
    };

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);

    motor->go_for(request->rpm(), request->revolutions());

    return ::grpc::Status();
}

::grpc::Status MotorServer::GoTo(::grpc::ServerContext* context,
                                 const ::viam::component::motor::v1::GoToRequest* request,
                                 ::viam::component::motor::v1::GoToResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::GoTo] without a request");
    };

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);

    motor->go_to(request->rpm(), request->position_revolutions());

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

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);

    motor->reset_zero_position(request->offset());

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

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);

    Motor::position result = motor->get_position();
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

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);

    Motor::properties result = motor->get_properties();
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

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);

    motor->stop(AttributeMap());

    return ::grpc::Status();
}

::grpc::Status MotorServer::IsPowered(::grpc::ServerContext* context,
                                      const ::viam::component::motor::v1::IsPoweredRequest* request,
                                      ::viam::component::motor::v1::IsPoweredResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::IsPowered] without a request");
    };

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);

    Motor::power_status result = motor->get_power_status();
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

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);

    bool result = motor->is_moving();
    response->set_is_moving(result);

    return ::grpc::Status();
}

::grpc::Status MotorServer::DoCommand(grpc::ServerContext* context,
                                      const viam::common::v1::DoCommandRequest* request,
                                      viam::common::v1::DoCommandResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::DoCommand] without a request");
    };

    std::shared_ptr<ResourceBase> rb = resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    AttributeMap result = motor->do_command(struct_to_map(request->command()));

    *response->mutable_result() = map_to_struct(result);

    return ::grpc::Status();
}

void MotorServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

}  // namespace sdk
}  // namespace viam
