#include <viam/sdk/components/private/base_client.hpp>

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/base/v1/base.grpc.pb.h>
#include <viam/api/component/base/v1/base.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/base.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {
namespace impl {

BaseClient::BaseClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Base(std::move(name)),
      stub_(viam::component::base::v1::BaseService::NewStub(channel)),
      channel_(std::move(channel)) {};

void BaseClient::move_straight(int64_t distance_mm, double mm_per_sec, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::MoveStraight)
        .with(extra,
              [&](auto& request) {
                  request.set_distance_mm(distance_mm);
                  request.set_mm_per_sec(mm_per_sec);
              })
        .invoke();
}

void BaseClient::spin(double angle_deg, double degs_per_sec, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Spin)
        .with(extra,
              [&](auto& request) {
                  request.set_angle_deg(angle_deg);
                  request.set_degs_per_sec(degs_per_sec);
              })
        .invoke();
}

void BaseClient::set_power(const Vector3& linear,
                           const Vector3& angular,
                           const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::SetPower)
        .with(extra,
              [&](auto& request) {
                  *request.mutable_linear() = linear.to_proto();
                  *request.mutable_angular() = angular.to_proto();
              })
        .invoke();
}

void BaseClient::set_velocity(const Vector3& linear,
                              const Vector3& angular,
                              const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::SetVelocity)
        .with(extra,
              [&](auto& request) {
                  *request.mutable_linear() = linear.to_proto();
                  *request.mutable_angular() = angular.to_proto();
              })
        .invoke();
}

void BaseClient::stop(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Stop).with(extra).invoke();
}

bool BaseClient::is_moving() {
    return make_client_helper(this, *stub_, &StubType::IsMoving).invoke([](auto& response) {
        return response.is_moving();
    });
}

std::vector<GeometryConfig> BaseClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return GeometryConfig::from_proto(response); });
}

Base::properties BaseClient::get_properties(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetProperties)
        .with(extra)
        .invoke([](auto& response) { return properties::from_proto(response); });
}

ProtoStruct BaseClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = map_to_struct(command); })
        .invoke([](auto& response) { return struct_to_map(response.result()); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
