#include <viam/sdk/components/private/gantry_client.hpp>

#include <grpcpp/channel.h>

#include <viam/api/component/gantry/v1/gantry.grpc.pb.h>
#include <viam/api/component/gantry/v1/gantry.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/kinematics.hpp>

namespace viam {
namespace sdk {
namespace impl {

using sdk::from_proto;
using sdk::to_proto;

GantryClient::GantryClient(std::string name, const ViamChannel& channel)
    : Gantry(std::move(name)),
      stub_(viam::component::gantry::v1::GantryService::NewStub(channel.channel())),
      channel_(&channel) {}

std::vector<double> GantryClient::get_position(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetPosition)
        .with(extra)
        .invoke([&](auto& response) {
            return std::vector<double>(response.positions_mm().begin(),
                                       response.positions_mm().end());
        });
}

void GantryClient::move_to_position(const std::vector<Gantry::movement_coordinate>& coordinates,
                                    const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::MoveToPosition)
        .with(extra,
              [&](auto& request) {
                  request.mutable_positions_mm()->Reserve(coordinates.size());
                  request.mutable_speeds_mm_per_sec()->Reserve(coordinates.size());

                  for (const auto& coord : coordinates) {
                      request.mutable_positions_mm()->Add(coord.position_mm);
                      request.mutable_speeds_mm_per_sec()->Add(coord.speed_mm_per_sec);
                  }
              })
        .invoke();
}

bool GantryClient::home(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Home).with(extra).invoke([](auto& response) {
        return response.homed();
    });
}

std::vector<double> GantryClient::get_lengths(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetLengths)
        .with(extra)
        .invoke([](auto& response) {
            return std::vector<double>(response.lengths_mm().begin(), response.lengths_mm().end());
        });
}

bool GantryClient::is_moving() {
    return make_client_helper(this, *stub_, &StubType::IsMoving).invoke([](auto& response) {
        return response.is_moving();
    });
}

void GantryClient::stop(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Stop).with(extra).invoke();
}

ProtoStruct GantryClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
}

::viam::sdk::KinematicsData GantryClient::get_kinematics(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetKinematics)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

std::vector<GeometryConfig> GantryClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
