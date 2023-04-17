#include <components/motor/server.hpp>

#include <common/utils.hpp>
#include <components/motor/motor.hpp>
#include <config/resource.hpp>
#include <rpc/server.hpp>



::grpc::Status MotorServer::SetPower(::grpc::ServerContext* context,
                        const ::viam::component::motor::v1::SetPowerRequest* request,
                        ::viam::component::motor::v1::SetPowerResponse* response) {
    
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::SetPower] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }
    
    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    return ::grpc::Status();
}

::grpc::Status MotorServer::GoFor(::grpc::ServerContext* context,
                        const ::viam::component::motor::v1::GoForRequest* request,
                        ::viam::component::motor::v1::GoForResponse* response) {
    
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::GoFor] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }
    
    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    return ::grpc::Status();
}

::grpc::Status MotorServer::GoTo(::grpc::ServerContext* context,
                        const ::viam::component::motor::v1::GoToRequest* request,
                        ::viam::component::motor::v1::GoToResponse* response) {
    
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::GoTo] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }
    
    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    return ::grpc::Status();
}

::grpc::Status MotorServer::ResetZeroPosition(::grpc::ServerContext* context,
                        const ::viam::component::motor::v1::ResetZeroPositionRequest* request,
                        ::viam::component::motor::v1::ResetZeroPositionResponse* response) {
    
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::ResetZeroPosition] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }
    
    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    return ::grpc::Status();
}

::grpc::Status MotorServer::GetPosition(::grpc::ServerContext* context,
                        const ::viam::component::motor::v1::GetPositionRequest* request,
                        ::viam::component::motor::v1::GetPositionResponse* response) {
    
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::GetPosition] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }
    
    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    
    Motor::position result = motor->get_position();
    *response->mutable_position() = result.position;
    return ::grpc::Status();
}

::grpc::Status MotorServer::GetProperties(::grpc::ServerContext* context,
                        const ::viam::component::motor::v1::GetPropertiesRequest* request,
                        ::viam::component::motor::v1::GetPropertiesResponse* response) {
    
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::GetProperties] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }
    
    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    
    Motor::properties result = motor->get_properties();
    *response->mutable_position_reporting() = result.position_reporting;
    return ::grpc::Status();
}

::grpc::Status MotorServer::Stop(::grpc::ServerContext* context,
                        const ::viam::component::motor::v1::StopRequest* request,
                        ::viam::component::motor::v1::StopResponse* response) {
    
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::Stop] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }
    
    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    return ::grpc::Status();
}

::grpc::Status MotorServer::IsPowered(::grpc::ServerContext* context,
                        const ::viam::component::motor::v1::IsPoweredRequest* request,
                        ::viam::component::motor::v1::IsPoweredResponse* response) {
    
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::IsPowered] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }
    
    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    
    Motor::power_status result = motor->get_power_status();
    *response->mutable_is_on() = result.is_on;
    *response->mutable_power_pct() = result.power_pct;
    return ::grpc::Status();
}

::grpc::Status MotorServer::IsMoving(::grpc::ServerContext* context,
                        const ::viam::component::motor::v1::IsMovingRequest* request,
                        ::viam::component::motor::v1::IsMovingResponse* response) {
    
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::IsMoving] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }
    
    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    
    Motor::moving_status result = motor->is_moving();
    *response->mutable_is_moving() = result.is_moving;
    return ::grpc::Status();
}

::grpc::Status MotorServer::DoCommand(::grpc::ServerContext* context,
                        const ::viam::component::motor::v1::common.v1.DoCommandRequest* request,
                        ::viam::component::motor::v1::common.v1.DoCommandResponse* response) {
    
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Motor::DoCommand] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }
    
    std::shared_ptr<Motor> motor = std::dynamic_pointer_cast<Motor>(rb);
    
    Motor::ERROR/TODO result = motor->do_command(request->TODO());
    return ::grpc::Status();
}


void MotorServer::register_server(std::shared_ptr<Server> server) {
    viam::component::motor::v1::MotorService::Service* motor =
        static_cast<viam::component::motor::v1::MotorService::Service*>(this);
    server->register_service(motor);
}

std::shared_ptr<SubtypeService> MotorServer::get_sub_svc() {
    return sub_svc;
}
