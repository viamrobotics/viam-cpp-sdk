#include <viam/sdk/components/private/gantry_server.hpp>

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
        std::vector<Gantry::movement_coordinate> coords;
        for (int i = 0;
             i < std::min(request->positions_mm_size(), request->speeds_mm_per_sec_size());
             ++i) {
            coords.push_back({request->positions_mm(i), request->speeds_mm_per_sec(i)});
        }

        gantry->move_to_position(coords, helper.getExtra());
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

}  // namespace impl
}  // namespace sdk
}  // namespace viam
