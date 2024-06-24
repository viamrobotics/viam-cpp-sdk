#include <viam/sdk/components/private/gantry_client.hpp>

#include <viam/api/component/gantry/v1/gantry.grpc.pb.h>
#include <viam/api/component/gantry/v1/gantry.pb.h>

#include "gantry_client.hpp"
#include <viam/sdk/common/client_helper.hpp>

namespace viam {
namespace sdk {
namespace impl {

GantryClient::GantryClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Gantry(std::move(name)),
      stub_(viam::component::gantry::v1::GantryService::NewStub(channel)),
      channel_(std::move(channel)) {}

std::vector<double> GantryClient::get_position(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetPosition)
        .with(extra)
        .invoke([&](auto& response) {
            return std::vector<double>(response.positions_mm().begin(),
                                       response.positions_mm().end());
        });
}

void GantryClient::move_to_position(const std::vector<Gantry::movement_coordinate>& coordinates,
                                    const AttributeMap& extra) {
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

bool GantryClient::home(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::Home).with(extra).invoke([](auto& response) {
        return response.homed();
    });
}

std::vector<double> GantryClient::get_lengths(const AttributeMap& extra) {
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

void GantryClient::stop(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::Stop).with(extra).invoke();
}

AttributeMap GantryClient::do_command(const AttributeMap& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
