#include <components/base/client.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include <common/v1/common.pb.h>
#include <component/base/v1/base.grpc.pb.h>

#include <common/utils.hpp>
#include <components/base/base.hpp>
#include <config/resource.hpp>
#include <robot/client.hpp>

AttributeMap BaseClient::do_command(AttributeMap command) {
    viam::common::v1::DoCommandRequest req;
    viam::common::v1::DoCommandResponse resp;
    grpc::ClientContext ctx;

    google::protobuf::Struct proto_command = map_to_struct(command);
    *req.mutable_command() = proto_command;
    *req.mutable_name() = this->name();
    stub_->DoCommand(&ctx, req, &resp);
    return struct_to_map(resp.result());
};

void BaseClient::move_straight(int64_t distance_mm, double mm_per_sec) {
    viam::component::base::v1::MoveStraightRequest req;
    viam::component::base::v1::MoveStraightResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();
    req.set_distance_mm(distance_mm);
    req.set_mm_per_sec(mm_per_sec);

    stub_->MoveStraight(&ctx, req, &resp);
};

void BaseClient::spin(double angle_deg, double degs_per_sec) {
    viam::component::base::v1::SpinRequest req;
    viam::component::base::v1::SpinResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();
    req.set_angle_deg(angle_deg);
    req.set_degs_per_sec(degs_per_sec);

    stub_->Spin(&ctx, req, &resp);
};

void BaseClient::stop() {
    viam::component::base::v1::StopRequest req;
    viam::component::base::v1::StopResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();

    stub_->Stop(&ctx, req, &resp);
};

void BaseClient::set_power(const viam::common::v1::Vector3& linear,
                           const viam::common::v1::Vector3& angular) {
    viam::component::base::v1::SetPowerRequest req;
    viam::component::base::v1::SetPowerResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();
    *req.mutable_linear() = linear;
    *req.mutable_angular() = angular;

    stub_->SetPower(&ctx, req, &resp);
};

void BaseClient::set_velocity(const viam::common::v1::Vector3& linear,
                              const viam::common::v1::Vector3& angular) {
    viam::component::base::v1::SetVelocityRequest req;
    viam::component::base::v1::SetVelocityResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();
    *req.mutable_linear() = linear;
    *req.mutable_angular() = angular;

    stub_->SetVelocity(&ctx, req, &resp);
};

bool BaseClient::is_moving() {
    viam::component::base::v1::IsMovingRequest req;
    viam::component::base::v1::IsMovingResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = this->name();

    stub_->IsMoving(&ctx, req, &resp);
    return resp.is_moving();
};
