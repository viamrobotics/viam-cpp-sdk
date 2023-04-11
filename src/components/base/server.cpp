#include <components/base/server.hpp>

#include <common/utils.hpp>
#include <components/base/base.hpp>
#include <config/resource.hpp>
#include <rpc/server.hpp>

::grpc::Status BaseServer::DoCommand(::grpc::ServerContext* context,
                                     const ::viam::common::v1::DoCommandRequest* request,
                                     ::viam::common::v1::DoCommandResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [DoCommand] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Base> base = std::dynamic_pointer_cast<Base>(rb);
    AttributeMap result = base->do_command(struct_to_map(request->command()));

    *response->mutable_result() = map_to_struct(result);

    return ::grpc::Status();
}

::grpc::Status BaseServer::MoveStraight(
    ::grpc::ServerContext* context,
    const ::viam::component::base::v1::MoveStraightRequest* request,
    ::viam::component::base::v1::MoveStraightResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [MoveStraight] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Base> base = std::dynamic_pointer_cast<Base>(rb);
    base->move_straight(request->distance_mm(), request->mm_per_sec());

    return ::grpc::Status();
}

::grpc::Status BaseServer::Spin(::grpc::ServerContext* context,
                                const ::viam::component::base::v1::SpinRequest* request,
                                ::viam::component::base::v1::SpinResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Spin] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Base> base = std::dynamic_pointer_cast<Base>(rb);
    base->spin(request->angle_deg(), request->degs_per_sec());

    return ::grpc::Status();
}

::grpc::Status BaseServer::SetPower(::grpc::ServerContext* context,
                                    const ::viam::component::base::v1::SetPowerRequest* request,
                                    ::viam::component::base::v1::SetPowerResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [SetPower] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Base> base = std::dynamic_pointer_cast<Base>(rb);
    base->set_power(proto_to_vector3(request->linear()), proto_to_vector3(request->angular()));

    return ::grpc::Status();
}

::grpc::Status BaseServer::SetVelocity(
    ::grpc::ServerContext* context,
    const ::viam::component::base::v1::SetVelocityRequest* request,
    ::viam::component::base::v1::SetVelocityResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [SetVelocity] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Base> base = std::dynamic_pointer_cast<Base>(rb);
    base->set_velocity(proto_to_vector3(request->linear()), proto_to_vector3(request->angular()));

    return ::grpc::Status();
}

::grpc::Status BaseServer::Stop(::grpc::ServerContext* context,
                                const ::viam::component::base::v1::StopRequest* request,
                                ::viam::component::base::v1::StopResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Stop] without a request");
    };

    std::shared_ptr<ResourceBase> rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Base> base = std::dynamic_pointer_cast<Base>(rb);
    base->stop();

    return ::grpc::Status();
}

::grpc::Status BaseServer::IsMoving(::grpc::ServerContext* context,
                                    const ::viam::component::base::v1::IsMovingRequest* request,
                                    ::viam::component::base::v1::IsMovingResponse* response) {
    if (request == nullptr) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [IsMoving] without a request");
    };

    rust

        std::shared_ptr<ResourceBase>
            rb = sub_svc->resource(request->name());
    if (rb == nullptr) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Base> base = std::dynamic_pointer_cast<Base>(rb);
    bool is_moving = base->is_moving();

    response->set_is_moving(is_moving);

    return ::grpc::Status();
}

void BaseServer::register_server() {
    viam::component::base::v1::BaseService::Service* base =
        static_castviam::component::base::v1::BaseService::Service * (this);
    Server::register_service(base);
}

std::shared_ptr<SubtypeService> BaseServer::get_sub_svc() {
    return sub_svc;
}
