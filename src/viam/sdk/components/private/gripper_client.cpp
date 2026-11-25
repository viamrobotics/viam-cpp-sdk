#include <viam/sdk/components/private/gripper_client.hpp>

#include <grpcpp/channel.h>

#include <viam/api/component/gripper/v1/gripper.grpc.pb.h>
#include <viam/api/component/gripper/v1/gripper.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {
namespace impl {

GripperClient::GripperClient(std::string name, const ViamChannel& channel)
    : Gripper(std::move(name)),
      stub_(viam::component::gripper::v1::GripperService::NewStub(channel.channel())),
      channel_(&channel) {}

void GripperClient::open(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Open).with(extra).invoke();
}

bool GripperClient::grab(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Grab).with(extra).invoke([](auto& response) {
        return response.success();
    });
}

Gripper::holding_status GripperClient::is_holding_something(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::IsHoldingSomething)
        .with(extra)
        .invoke([](auto& response) {
            Gripper::holding_status holding_status;
            holding_status.is_holding_something = response.is_holding_something();
            holding_status.meta = from_proto(response.meta());
            return holding_status;
        });
}

bool GripperClient::is_moving() {
    return make_client_helper(this, *stub_, &StubType::IsMoving).invoke([](auto& response) {
        return response.is_moving();
    });
}

void GripperClient::stop(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Stop).with(extra).invoke();
}

ProtoStruct GripperClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
}

std::vector<GeometryConfig> GripperClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

ProtoStruct GripperClient::get_status() {
    return make_client_helper(this, *stub_, &StubType::GetStatus).invoke([](auto& response) {
        return from_proto(response.result());
    });
}

void GripperClient::set_percent_open(double percent_open, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::SetPercentOpen)
        .with(extra)
        .with([&](auto& request) { request.set_percent_open(percent_open); })
        .invoke();
}

double GripperClient::get_percent_open(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetPercentOpen)
        .with(extra)
        .invoke([](auto& response) { return response.percent_open(); });
}

Gripper::properties GripperClient::get_properties(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetProperties)
        .with(extra)
        .invoke([](auto& response) {
            Gripper::properties props;
            props.holds_something = response.holds_something();
            props.can_grasp = response.can_grasp();
            return props;
        });
}

// Specializations for from_proto
template <>
Gripper::holding_status from_proto(const viam::component::gripper::v1::IsHoldingSomethingResponse& proto) {
    Gripper::holding_status holding_status;
    holding_status.is_holding_something = proto.is_holding_something();
    holding_status.meta = from_proto(proto.meta());
    return holding_status;
}

template <>
Gripper::properties from_proto(const viam::component::gripper::v1::GetPropertiesResponse& proto) {
    Gripper::properties props;
    props.holds_something = proto.holds_something();
    props.can_grasp = proto.can_grasp();
    return props;
}

template <>
double from_proto(const viam::component::gripper::v1::GetPercentOpenResponse& proto) {
    return proto.percent_open();
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam