#include <viam/sdk/components/base/server.hpp>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/base/base.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

::grpc::Status BaseServer::MoveStraight(
    ::grpc::ServerContext* context,
    const ::viam::component::base::v1::MoveStraightRequest* request,
    ::viam::component::base::v1::MoveStraightResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Base::MoveStraight] without a request");
    };

    auto rb = get_sub_svc()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Base> base = std::dynamic_pointer_cast<Base>(rb);

    base->move_straight(request->distance_mm(), request->mm_per_sec());

    return ::grpc::Status();
}

::grpc::Status BaseServer::Spin(::grpc::ServerContext* context,
                                const ::viam::component::base::v1::SpinRequest* request,
                                ::viam::component::base::v1::SpinResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Base::Spin] without a request");
    };

    auto rb = get_sub_svc()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Base> base = std::dynamic_pointer_cast<Base>(rb);

    base->spin(request->angle_deg(), request->degs_per_sec());

    return ::grpc::Status();
}

::grpc::Status BaseServer::SetPower(::grpc::ServerContext* context,
                                    const ::viam::component::base::v1::SetPowerRequest* request,
                                    ::viam::component::base::v1::SetPowerResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Base::SetPower] without a request");
    };

    auto rb = get_sub_svc()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Base> base = std::dynamic_pointer_cast<Base>(rb);

    Vector3 linear = {request->linear().x(), request->linear().y(), request->linear().z()};
    Vector3 angular = {request->angular().x(), request->angular().y(), request->angular().z()};
    base->set_power(linear, angular);

    return ::grpc::Status();
}

::grpc::Status BaseServer::SetVelocity(
    ::grpc::ServerContext* context,
    const ::viam::component::base::v1::SetVelocityRequest* request,
    ::viam::component::base::v1::SetVelocityResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Base::SetVelocity] without a request");
    };

    auto rb = get_sub_svc()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Base> base = std::dynamic_pointer_cast<Base>(rb);

    Vector3 linear = {request->linear().x(), request->linear().y(), request->linear().z()};
    Vector3 angular = {request->angular().x(), request->angular().y(), request->angular().z()};
    base->set_velocity(linear, angular);

    return ::grpc::Status();
}

::grpc::Status BaseServer::Stop(::grpc::ServerContext* context,
                                const ::viam::component::base::v1::StopRequest* request,
                                ::viam::component::base::v1::StopResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Base::Stop] without a request");
    };

    auto rb = get_sub_svc()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Base> base = std::dynamic_pointer_cast<Base>(rb);

    base->stop();

    return ::grpc::Status();
}

::grpc::Status BaseServer::IsMoving(::grpc::ServerContext* context,
                                    const ::viam::component::base::v1::IsMovingRequest* request,
                                    ::viam::component::base::v1::IsMovingResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Base::IsMoving] without a request");
    };

    auto rb = get_sub_svc()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Base> base = std::dynamic_pointer_cast<Base>(rb);

    bool result = base->is_moving();
    response->set_is_moving(result);

    return ::grpc::Status();
}

::grpc::Status BaseServer::DoCommand(grpc::ServerContext* context,
                                     const viam::common::v1::DoCommandRequest* request,
                                     viam::common::v1::DoCommandResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Base::DoCommand] without a request");
    };

    auto rb = get_sub_svc()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    std::shared_ptr<Base> motor = std::dynamic_pointer_cast<Base>(rb);
    AttributeMap result = motor->do_command(struct_to_map(request->command()));

    *response->mutable_result() = map_to_struct(result);

    return ::grpc::Status();
}

void BaseServer::register_server(std::shared_ptr<Server> server) {
    viam::component::base::v1::BaseService::Service* base =
        static_cast<viam::component::base::v1::BaseService::Service*>(this);
    server->register_service(base);
}

std::shared_ptr<SubtypeService> BaseServer::get_sub_svc() {
    return sub_svc;
}

}  // namespace sdk
}  // namespace viam
