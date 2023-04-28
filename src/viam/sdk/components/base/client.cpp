#include <viam/sdk/components/base/client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/base/v1/base.grpc.pb.h>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/base/base.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

void BaseClient::move_straight(int64_t distance_mm, double mm_per_sec) {
    viam::component::base::v1::MoveStraightRequest request;
    viam::component::base::v1::MoveStraightResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_distance_mm(distance_mm);
    request.set_mm_per_sec(mm_per_sec);

    grpc::Status status = stub_->MoveStraight(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
}

void BaseClient::spin(double angle_deg, double degs_per_sec) {
    viam::component::base::v1::SpinRequest request;
    viam::component::base::v1::SpinResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_angle_deg(angle_deg);
    request.set_degs_per_sec(degs_per_sec);

    grpc::Status status = stub_->Spin(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
}

void BaseClient::set_power(Vector3 linear, Vector3 angular) {
    viam::component::base::v1::SetPowerRequest request;
    viam::component::base::v1::SetPowerResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.mutable_linear()->set_x(linear.at(0));
    request.mutable_linear()->set_y(linear.at(1));
    request.mutable_linear()->set_z(linear.at(2));
    request.mutable_angular()->set_x(angular.at(0));
    request.mutable_angular()->set_y(angular.at(1));
    request.mutable_angular()->set_z(angular.at(2));

    grpc::Status status = stub_->SetPower(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
}

void BaseClient::set_velocity(std::array<double, 3> linear, std::array<double, 3> angular) {
    viam::component::base::v1::SetVelocityRequest request;
    viam::component::base::v1::SetVelocityResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.mutable_linear()->set_x(linear.at(0));
    request.mutable_linear()->set_y(linear.at(1));
    request.mutable_linear()->set_z(linear.at(2));
    request.mutable_angular()->set_x(angular.at(0));
    request.mutable_angular()->set_y(angular.at(1));
    request.mutable_angular()->set_z(angular.at(2));

    grpc::Status status = stub_->SetVelocity(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
}

grpc::StatusCode BaseClient::stop() {
    viam::component::base::v1::StopRequest request;
    viam::component::base::v1::StopResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();

    grpc::Status status = stub_->Stop(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return status.error_code();
}

bool BaseClient::is_moving() {
    viam::component::base::v1::IsMovingRequest request;
    viam::component::base::v1::IsMovingResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();

    grpc::Status status = stub_->IsMoving(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return response.is_moving();
}

AttributeMap BaseClient::do_command(AttributeMap command) {
    viam::common::v1::DoCommandRequest request;
    viam::common::v1::DoCommandResponse response;

    grpc::ClientContext ctx;

    google::protobuf::Struct proto_command = map_to_struct(command);
    *request.mutable_command() = proto_command;
    *request.mutable_name() = this->name();

    grpc::Status status = stub_->DoCommand(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }
    return struct_to_map(response.result());
}

}  // namespace sdk
}  // namespace viam
