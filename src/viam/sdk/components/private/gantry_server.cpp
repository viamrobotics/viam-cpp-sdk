#include <viam/sdk/components/private/gantry_server.hpp>

#include <sstream>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/kinematics.hpp>
#include <viam/sdk/common/private/kinematics_response.hpp>
#include <viam/sdk/common/private/service_helper.hpp>

namespace viam {
namespace sdk {
namespace impl {

GantryServer::GantryServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status GantryServer::GetPosition(
    ::grpc::ServerContext* context,
    const ::viam::component::gantry::v1::GetPositionRequest* request,
    ::viam::component::gantry::v1::GetPositionResponse* response) noexcept {
    return make_service_helper<Gantry>(
        "GantryServer::GetPosition", this, context, request)([&](auto& helper, auto& gantry) {
        const std::vector<double> positions = gantry->get_position(helper.getExtra());
        *(response->mutable_positions_mm()) = {positions.begin(), positions.end()};
    });
}

::grpc::Status GantryServer::MoveToPosition(
    ::grpc::ServerContext* context,
    const ::viam::component::gantry::v1::MoveToPositionRequest* request,
    ::viam::component::gantry::v1::MoveToPositionResponse*) noexcept {
    return make_service_helper<Gantry>(
        "GantryServer::MoveToPosition", this, context, request)([&](auto& helper, auto& gantry) {
        // The RDK gantry client's GoToInputs (used by the motion service) sends no speeds at all,
        // so an empty speed list is accepted and every speed passed as 0. Any other speed list must
        // match the positions one-to-one, as the proto requires.
        const bool has_speeds = request->speeds_mm_per_sec_size() != 0;
        if (has_speeds && request->speeds_mm_per_sec_size() != request->positions_mm_size()) {
            std::ostringstream message;
            message << "Got " << request->speeds_mm_per_sec_size() << " speeds for "
                    << request->positions_mm_size()
                    << " positions; speeds must be omitted or match positions in length";
            return helper.fail(::grpc::INVALID_ARGUMENT, message.str().c_str());
        }

        std::vector<Gantry::movement_coordinate> coords;
        coords.reserve(request->positions_mm_size());
        for (int i = 0; i < request->positions_mm_size(); ++i) {
            coords.push_back(
                {request->positions_mm(i), has_speeds ? request->speeds_mm_per_sec(i) : 0.0});
        }

        gantry->move_to_position(coords, helper.getExtra());
        return ::grpc::Status();
    });
}

::grpc::Status GantryServer::Home(::grpc::ServerContext* context,
                                  const ::viam::component::gantry::v1::HomeRequest* request,
                                  ::viam::component::gantry::v1::HomeResponse* response) noexcept {
    return make_service_helper<Gantry>("GantryServer::Home", this, context, request)(
        [&](auto& helper, auto& gantry) { response->set_homed(gantry->home(helper.getExtra())); });
}

::grpc::Status GantryServer::GetLengths(
    ::grpc::ServerContext* context,
    const ::viam::component::gantry::v1::GetLengthsRequest* request,
    ::viam::component::gantry::v1::GetLengthsResponse* response) noexcept {
    return make_service_helper<Gantry>(
        "GantryServer::GetLengths", this, context, request)([&](auto& helper, auto& gantry) {
        const std::vector<double> lengths = gantry->get_lengths(helper.getExtra());
        *(response->mutable_lengths_mm()) = {lengths.begin(), lengths.end()};
    });
}

::grpc::Status GantryServer::Stop(::grpc::ServerContext* context,
                                  const ::viam::component::gantry::v1::StopRequest* request,
                                  ::viam::component::gantry::v1::StopResponse*) noexcept {
    return make_service_helper<Gantry>("GantryServer::Stop", this, context, request)(
        [&](auto& helper, auto& gantry) { gantry->stop(helper.getExtra()); });
}

::grpc::Status GantryServer::IsMoving(
    ::grpc::ServerContext* context,
    const ::viam::component::gantry::v1::IsMovingRequest* request,
    ::viam::component::gantry::v1::IsMovingResponse* response) noexcept {
    return make_service_helper<Gantry>("GantryServer::IsMoving", this, context, request)(
        [&](auto&, auto& gantry) { response->set_is_moving(gantry->is_moving()); });
}

::grpc::Status GantryServer::DoCommand(::grpc::ServerContext* context,
                                       const ::viam::common::v1::DoCommandRequest* request,
                                       ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Gantry>(
        "GantryServer::DoCommand", this, context, request)([&](auto&, auto& gantry) {
        const ProtoStruct result = gantry->do_command(from_proto(request->command()));
        *response->mutable_result() = to_proto(result);
    });
}

::grpc::Status GantryServer::GetKinematics(
    ::grpc::ServerContext* context,
    const ::viam::common::v1::GetKinematicsRequest* request,
    ::viam::common::v1::GetKinematicsResponse* response) noexcept {
    return make_service_helper<Gantry>(
        "GantryServer::GetKinematics", this, context, request)([&](auto& helper, auto& gantry) {
        const ProtoStruct extra = helper.getExtra();
        // Base get_kinematics throws k_not_supported since it is deprecated. For implementations
        // that still override it, raw byte fields go over the wire to serve older viam-server or
        // older clients that predate `model`.
        try {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
            *response = to_proto(gantry->get_kinematics(extra));
#pragma GCC diagnostic pop
        } catch (const Exception& xcp) {
            if (xcp.condition() != ErrorCondition::k_not_supported) {
                throw;
            }
        }
        const kinematics_options options{request->include_collision_meshes(),
                                         request->include_visual_meshes()};
        impl::set_response_model(
            gantry->get_kinematic_model(options, extra), gantry->name(), response);
    });
}

::grpc::Status GantryServer::GetGeometries(
    ::grpc::ServerContext* context,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<Gantry>(
        "GantryServer::GetGeometries", this, context, request)([&](auto& helper, auto& gantry) {
        const std::vector<GeometryConfig> geometries = gantry->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = to_proto(geometry);
        }
    });
}

::grpc::Status GantryServer::GetStatus(::grpc::ServerContext* context,
                                       const ::viam::common::v1::GetStatusRequest* request,
                                       ::viam::common::v1::GetStatusResponse* response) noexcept {
    return make_service_helper<Gantry>(
        "GantryServer::GetStatus", this, context, request)([&](auto&, auto& gantry) {
        const ProtoStruct result = gantry->get_status();
        *response->mutable_result() = to_proto(result);
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
