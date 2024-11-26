#include <viam/sdk/components/private/base_server.hpp>

#include <viam/api/component/base/v1/base.pb.h>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/base.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {
namespace impl {

BaseServer::BaseServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status BaseServer::MoveStraight(
    ::grpc::ServerContext*,
    const ::viam::component::base::v1::MoveStraightRequest* request,
    ::viam::component::base::v1::MoveStraightResponse*) noexcept {
    return make_service_helper<Base>(
        "BaseServer::MoveStraight", this, request)([&](auto& helper, auto& base) {
        base->move_straight(request->distance_mm(), request->mm_per_sec(), helper.getExtra());
    });
}

::grpc::Status BaseServer::Spin(::grpc::ServerContext*,
                                const ::viam::component::base::v1::SpinRequest* request,
                                ::viam::component::base::v1::SpinResponse*) noexcept {
    return make_service_helper<Base>(
        "BaseServer::Spin", this, request)([&](auto& helper, auto& base) {
        base->spin(request->angle_deg(), request->degs_per_sec(), helper.getExtra());
    });
}

::grpc::Status BaseServer::SetPower(::grpc::ServerContext*,
                                    const ::viam::component::base::v1::SetPowerRequest* request,
                                    ::viam::component::base::v1::SetPowerResponse*) noexcept {
    return make_service_helper<Base>(
        "BaseServer::SetPower", this, request)([&](auto& helper, auto& base) {
        auto linear = v2::from_proto(request->linear());
        auto angular = v2::from_proto(request->angular());
        base->set_power(linear, angular, helper.getExtra());
    });
}

::grpc::Status BaseServer::SetVelocity(
    ::grpc::ServerContext*,
    const ::viam::component::base::v1::SetVelocityRequest* request,
    ::viam::component::base::v1::SetVelocityResponse*) noexcept {
    return make_service_helper<Base>(
        "BaseServer::SetVelocity", this, request)([&](auto& helper, auto& base) {
        auto linear = v2::from_proto(request->linear());
        auto angular = v2::from_proto(request->angular());
        base->set_velocity(linear, angular, helper.getExtra());
    });
}

::grpc::Status BaseServer::Stop(::grpc::ServerContext*,
                                const ::viam::component::base::v1::StopRequest* request,
                                ::viam::component::base::v1::StopResponse*) noexcept {
    return make_service_helper<Base>("BaseServer::Stop", this, request)(
        [&](auto& helper, auto& base) { base->stop(helper.getExtra()); });
}

::grpc::Status BaseServer::IsMoving(
    ::grpc::ServerContext*,
    const ::viam::component::base::v1::IsMovingRequest* request,
    ::viam::component::base::v1::IsMovingResponse* response) noexcept {
    return make_service_helper<Base>("BaseServer::IsMoving", this, request)([&](auto&, auto& base) {
        const bool result = base->is_moving();
        response->set_is_moving(result);
    });
}

::grpc::Status BaseServer::GetGeometries(
    ::grpc::ServerContext*,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<Base>(
        "BaseServer::GetGeometries", this, request)([&](auto& helper, auto& base) {
        const std::vector<GeometryConfig> geometries = base->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = v2::to_proto(geometry);
        }
    });
}

::grpc::Status BaseServer::GetProperties(
    grpc::ServerContext*,
    const viam::component::base::v1::GetPropertiesRequest* request,
    viam::component::base::v1::GetPropertiesResponse* response) noexcept {
    return make_service_helper<Base>(
        "BaseServer::GetProperties", this, request)([&](auto& helper, auto& base) {
        const Base::properties result = base->get_properties(helper.getExtra());
        response->set_width_meters(result.width_meters);
        response->set_turning_radius_meters(result.turning_radius_meters);
        response->set_wheel_circumference_meters(result.wheel_circumference_meters);
    });
}

::grpc::Status BaseServer::DoCommand(grpc::ServerContext*,
                                     const viam::common::v1::DoCommandRequest* request,
                                     viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Base>(
        "BaseServer::DoCommand", this, request)([&](auto&, auto& base) {
        const ProtoStruct result = base->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
