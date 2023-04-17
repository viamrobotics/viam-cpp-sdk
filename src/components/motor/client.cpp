#include <components/motor/client.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include <common/v1/common.pb.h>
#include <component/motor/v1/motor.grpc.pb.h>

#include <common/utils.hpp>
#include <components/motor/motor.hpp>
#include <config/resource.hpp>
#include <robot/client.hpp>



void MotorClient::set_power(double power_pct){
    viam::component::motor::v1::SetPowerRequest request;
    viam::component::motor::v1::SetPowerResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_power_pct(power_pct);

    stub_->SetPower(&ctx, request, &response);
    return from_proto(response);
}

void MotorClient::go_for(double rpm, double revolutions){
    viam::component::motor::v1::GoForRequest request;
    viam::component::motor::v1::GoForResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_rpm(rpm);
    request.set_revolutions(revolutions);

    stub_->GoFor(&ctx, request, &response);
    return from_proto(response);
}

void MotorClient::go_to(double rpm, double position_revolutions){
    viam::component::motor::v1::GoToRequest request;
    viam::component::motor::v1::GoToResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_rpm(rpm);
    request.set_position_revolutions(position_revolutions);

    stub_->GoTo(&ctx, request, &response);
    return from_proto(response);
}

void MotorClient::reset_zero_position(double offset){
    viam::component::motor::v1::ResetZeroPositionRequest request;
    viam::component::motor::v1::ResetZeroPositionResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_offset(offset);

    stub_->ResetZeroPosition(&ctx, request, &response);
    return from_proto(response);
}

Motor::position MotorClient::get_position(){
    viam::component::motor::v1::GetPositionRequest request;
    viam::component::motor::v1::GetPositionResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();

    stub_->GetPosition(&ctx, request, &response);
    return from_proto(response);
}

Motor::properties MotorClient::get_properties(){
    viam::component::motor::v1::GetPropertiesRequest request;
    viam::component::motor::v1::GetPropertiesResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();

    stub_->GetProperties(&ctx, request, &response);
    return from_proto(response);
}

void MotorClient::stop(){
    viam::component::motor::v1::StopRequest request;
    viam::component::motor::v1::StopResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();

    stub_->Stop(&ctx, request, &response);
    return from_proto(response);
}

Motor::power_status MotorClient::get_power_status(){
    viam::component::motor::v1::IsPoweredRequest request;
    viam::component::motor::v1::IsPoweredResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();

    stub_->IsPowered(&ctx, request, &response);
    return from_proto(response);
}

Motor::moving_status MotorClient::is_moving(){
    viam::component::motor::v1::IsMovingRequest request;
    viam::component::motor::v1::IsMovingResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();

    stub_->IsMoving(&ctx, request, &response);
    return from_proto(response);
}

AttributeMap MotorClient::do_command(ERROR TODO){
    viam::component::motor::v1::common.v1.DoCommandRequest request;
    viam::component::motor::v1::common.v1.DoCommandResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_TODO(TODO);

    stub_->DoCommand(&ctx, request, &response);
    return from_proto(response);
}


