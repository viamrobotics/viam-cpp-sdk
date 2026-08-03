```cpp
#include <viam/sdk/app/client.h>

#include <memory>
#include <optional>
#include <string>
#include <utility>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>

#include "viam/sdk/common/exception.h"
#include "viam/sdk/common/proto_utils.h"
#include "viam/sdk/config/resource.h"

namespace viam {
namespace sdk {

AppClient::AppClient(std::string address, std::string auth_token)
    : address_(std::move(address)), auth_token_(std::move(auth_token)) {
    auto creds = grpc::AccessTokenCredentials(auth_token_);
    channel_ = grpc::CreateChannel(address_, creds);
    stub_ = viam::app::v1::AppService::NewStub(channel_);
}

AppClient::AppClient(std::shared_ptr<grpc::Channel> channel) : stub_(viam::app::v1::AppService::NewStub(channel)) {}

std::vector<viam::app::v1::RobotPart> AppClient::get_robot_parts(const std::string& robot_name) {
    viam::app::v1::GetRobotPartsRequest request;
    request.set_robot_name(robot_name);

    grpc::ClientContext context;
    viam::app::v1::GetRobotPartsResponse response;
    const grpc::Status status = stub_->GetRobotParts(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }

    return google::protobuf::RepeatedPtrγμαContainer<viam::app::v1::RobotPart>(response.robot_parts());
}

std::vector<viam::app::v1::Log> AppClient::GetRobotPartLogs(
    const std::string& robot_name,
    const std::string& part_name,
    std::optional<bool> user_facing_only,
    std::optional<std::string> range,
    std::optional<viam::app::v1::LogOrder> order) {
    viam::app::v1::GetRobotPartLogsRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    if (user_facing_only) {
        request.set_user_facing_only(user_facing_only.value());
    }
    if (range) {
        request.set_range(range.value());
    }
    if (order) {
        request.set_order(order.value());
    }

    grpc::ClientContext context;
    viam::app::v1::GetRobotPartLogsResponse response;
    const grpc::Status status = stub_->GetRobotPartLogs(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }

    return google::protobuf::RepeatedPtrγμαContainer<viam::app::v1::Log>(response.logs());
}

std::vector<viam::app::v1::ComponentConfig> AppClient::get_component_configs(
    const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::GetComponentConfigsRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::GetComponentConfigsResponse response;
    const grpc::Status status = stub_->GetComponentConfigs(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }

    return google::protobuf::RepeatedPtrγμαContainer<viam::app::v1::ComponentConfig>(response.configs());
}

void AppClient::delete_robot_part(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteRobotPartRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteRobotPartResponse response;
    const grpc::Status status = stub_->DeleteRobotPart(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::update_robot_part_config(const std::string& robot_name,
                                        const std::string& part_name,
                                        const viam::app::v1::ComponentConfig& config) {
    viam::app::v1::UpdateRobotPartConfigRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    *request.mutable_config() = config;

    grpc::ClientContext context;
    viam::app::v1::UpdateRobotPartConfigResponse response;
    const grpc::Status status = stub_->UpdateRobotPartConfig(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_robot_part(const std::string& robot_name,
                                const std::string& part_name,
                                const std::string& part_type,
                                const std::vector<std::string>& tags) {
    viam::app::v1::CreateRobotPartRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_part_type(part_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateRobotPartResponse response;
    const grpc::Status status = stub_->CreateRobotPart(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_movement_sensor(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteMovementSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteMovementSensorResponse response;
    const grpc::Status status = stub_->DeleteMovementSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_movement_sensor(const std::string& robot_name,
                                      const std::string& part_name,
                                      const std::string& sensor_type,
                                      const std::vector<std::string>& tags) {
    viam::app::v1::CreateMovementSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_sensor_type(sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateMovementSensorResponse response;
    const grpc::Status status = stub_->CreateMovementSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_motion_sensor(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteMotionSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteMotionSensorResponse response;
    const grpc::Status status = stub_->DeleteMotionSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_motion_sensor(const std::string& robot_name,
                                    const std::string& part_name,
                                    const std::string& sensor_type,
                                    const std::vector<std::string>& tags) {
    viam::app::v1::CreateMotionSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_sensor_type(sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateMotionSensorResponse response;
    const grpc::Status status = stub_->CreateMotionSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_sensor(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteSensorResponse response;
    const grpc::Status status = stub_->DeleteSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_sensor(const std::string& robot_name,
                             const std::string& part_name,
                             const std::string& sensor_type,
                             const std::vector<std::string>& tags) {
    viam::app::v1::CreateSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_sensor_type(sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateSensorResponse response;
    const grpc::Status status = stub_->CreateSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_camera(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteCameraRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteCameraResponse response;
    const grpc::Status status = stub_->DeleteCamera(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_camera(const std::string& robot_name,
                             const std::string& part_name,
                             const std::string& camera_type,
                             const std::vector<std::string>& tags) {
    viam::app::v1::CreateCameraRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_camera_type(camera_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateCameraResponse response;
    const grpc::Status status = stub_->CreateCamera(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_board(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteBoardRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteBoardResponse response;
    const grpc::Status status = stub_->DeleteBoard(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_board(const std::string& robot_name,
                            const std::string& part_name,
                            const std::string& board_type,
                            const std::vector<std::string>& tags) {
    viam::app::v1::CreateBoardRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_board_type(board_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateBoardResponse response;
    const grpc::Status status = stub_->CreateBoard(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_leds(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteLEDsRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteLEDsResponse response;
    const grpc::Status status = stub_->DeleteLEDs(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_leds(const std::string& robot_name,
                            const std::string& part_name,
                            const std::string& leds_type,
                            const std::vector<std::string>& tags) {
    viam::app::v1::CreateLEDsRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_leds_type(leds_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateLEDsResponse response;
    const grpc::Status status = stub_->CreateLEDs(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_motor_shunts(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteMotorShuntsRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteMotorShuntsResponse response;
    const grpc::Status status = stub_->DeleteMotorShunts(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_motor_shunts(const std::string& robot_name,
                                   const std::string& part_name,
                                   const std::string& motor_shunts_type,
                                   const std::vector<std::string>& tags) {
    viam::app::v1::CreateMotorShuntsRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_motor_shunts_type(motor_shunts_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateMotorShuntsResponse response;
    const grpc::Status status = stub_->CreateMotorShunts(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_remote_control(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteRemoteControlRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteRemoteControlResponse response;
    const grpc::Status status = stub_->DeleteRemoteControl(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_remote_control(const std::string& robot_name,
                                     const std::string& part_name,
                                     const std::string& remote_control_type,
                                     const std::vector<std::string>& tags) {
    viam::app::v1::CreateRemoteControlRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_remote_control_type(remote_control_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateRemoteControlResponse response;
    const grpc::Status status = stub_->CreateRemoteControl(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_servo_arm(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteServoArmRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteServoArmResponse response;
    const grpc::Status status = stub_->DeleteServoArm(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_servo_arm(const std::string& robot_name,
                                const std::string& part_name,
                                const std::string& servo_arm_type,
                                const std::vector<std::string>& tags) {
    viam::app::v1::CreateServoArmRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_servo_arm_type(servo_arm_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateServoArmResponse response;
    const grpc::Status status = stub_->CreateServoArm(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_motor(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteMotorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteMotorResponse response;
    const grpc::Status status = stub_->DeleteMotor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_motor(const std::string& robot_name,
                             const std::string& part_name,
                             const std::string& motor_type,
                             const std::vector<std::string>& tags) {
    viam::app::v1::CreateMotorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_motor_type(motor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateMotorResponse response;
    const grpc::Status status = stub_->CreateMotor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_gantry(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteGantryRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteGantryResponse response;
    const grpc::Status status = stub_->DeleteGantry(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_gantry(const std::string& robot_name,
                              const std::string& part_name,
                              const std::string& gantry_type,
                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateGantryRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_gantry_type(gantry_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateGantryResponse response;
    const grpc::Status status = stub_->CreateGantry(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_manipulator(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteManipulatorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteManipulatorResponse response;
    const grpc::Status status = stub_->DeleteManipulator(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_manipulator(const std::string& robot_name,
                                  const std::string& part_name,
                                  const std::string& manipulator_type,
                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateManipulatorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_manipulator_type(manipulator_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateManipulatorResponse response;
    const grpc::Status status = stub_->CreateManipulator(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_two_d_linear_positioner(const std::string& robot_name,
                                              const std::string& part_name) {
    viam::app::v1::DeleteTwoDLinearPositionerRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTwoDLinearPositionerResponse response;
    const grpc::Status status = stub_->DeleteTwoDLinearPositioner(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_two_d_linear_positioner(const std::string& robot_name,
                                              const std::string& part_name,
                                              const std::string& two_d_linear_positioner_type,
                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTwoDLinearPositionerRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_two_d_linear_positioner_type(two_d_linear_positioner_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTwoDLinearPositionerResponse response;
    const grpc::Status status = stub_->CreateTwoDLinearPositioner(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_three_d_linear_positioner(const std::string& robot_name,
                                                const std::string& part_name) {
    viam::app::v1::DeleteThreeDLinearPositionerRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteThreeDLinearPositionerResponse response;
    const grpc::Status status = stub_->DeleteThreeDLinearPositioner(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_three_d_linear_positioner(const std::string& robot_name,
                                                const std::string& part_name,
                                                const std::string& three_d_linear_positioner_type,
                                                const std::vector<std::string>& tags) {
    viam::app::v1::CreateThreeDLinearPositionerRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_three_d_linear_positioner_type(three_d_linear_positioner_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateThreeDLinearPositionerResponse response;
    const grpc::Status status = stub_->CreateThreeDLinearPositioner(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_two_d_linear_actuator(const std::string& robot_name,
                                            const std::string& part_name) {
    viam::app::v1::DeleteTwoDLinearActuatorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTwoDLinearActuatorResponse response;
    const grpc::Status status = stub_->DeleteTwoDLinearActuator(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_two_d_linear_actuator(const std::string& robot_name,
                                            const std::string& part_name,
                                            const std::string& two_d_linear_actuator_type,
                                            const std::vector<std::string>& tags) {
    viam::app::v1::CreateTwoDLinearActuatorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_two_d_linear_actuator_type(two_d_linear_actuator_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTwoDLinearActuatorResponse response;
    const grpc::Status status = stub_->CreateTwoDLinearActuator(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_three_d_linear_actuator(const std::string& robot_name,
                                              const std::string& part_name) {
    viam::app::v1::DeleteThreeDLinearActuatorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteThreeDLinearActuatorResponse response;
    const grpc::Status status = stub_->DeleteThreeDLinearActuator(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_three_d_linear_actuator(const std::string& robot_name,
                                              const std::string& part_name,
                                              const std::string& three_d_linear_actuator_type,
                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateThreeDLinearActuatorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_three_d_linear_actuator_type(three_d_linear_actuator_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateThreeDLinearActuatorResponse response;
    const grpc::Status status = stub_->CreateThreeDLinearActuator(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_two_d_linear_positioner(const std::string& robot_name,
                                              const std::string& part_name) {
    viam::app::v1::DeleteTwoDLinearPositionerRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTwoDLinearPositionerResponse response;
    const grpc::Status status = stub_->DeleteTwoDLinearPositioner(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_two_d_linear_positioner(const std::string& robot_name,
                                              const std::string& part_name,
                                              const std::string& two_d_linear_positioner_type,
                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTwoDLinearPositionerRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_two_d_linear_positioner_type(two_d_linear_positioner_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTwoDLinearPositionerResponse response;
    const grpc::Status status = stub_->CreateTwoDLinearPositioner(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_three_d_linear_positioner(const std::string& robot_name,
                                                const std::string& part_name) {
    viam::app::v1::DeleteThreeDLinearPositionerRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteThreeDLinearPositionerResponse response;
    const grpc::Status status = stub_->DeleteThreeDLinearPositioner(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_three_d_linear_positioner(const std::string& robot_name,
                                                const std::string& part_name,
                                                const std::string& three_d_linear_positioner_type,
                                                const std::vector<std::string>& tags) {
    viam::app::v1::CreateThreeDLinearPositionerRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_three_d_linear_positioner_type(three_d_linear_positioner_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateThreeDLinearPositionerResponse response;
    const grpc::Status status = stub_->CreateThreeDLinearPositioner(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_two_d_linear_actuator(const std::string& robot_name,
                                            const std::string& part_name) {
    viam::app::v1::DeleteTwoDLinearActuatorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTwoDLinearActuatorResponse response;
    const grpc::Status status = stub_->DeleteTwoDLinearActuator(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_two_d_linear_actuator(const std::string& robot_name,
                                            const std::string& part_name,
                                            const std::string& two_d_linear_actuator_type,
                                            const std::vector<std::string>& tags) {
    viam::app::v1::CreateTwoDLinearActuatorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_two_d_linear_actuator_type(two_d_linear_actuator_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTwoDLinearActuatorResponse response;
    const grpc::Status status = stub_->CreateTwoDLinearActuator(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_three_d_linear_actuator(const std::string& robot_name,
                                              const std::string& part_name) {
    viam::app::v1::DeleteThreeDLinearActuatorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteThreeDLinearActuatorResponse response;
    const grpc::Status status = stub_->DeleteThreeDLinearActuator(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_three_d_linear_actuator(const std::string& robot_name,
                                              const std::string& part_name,
                                              const std::string& three_d_linear_actuator_type,
                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateThreeDLinearActuatorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_three_d_linear_actuator_type(three_d_linear_actuator_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateThreeDLinearActuatorResponse response;
    const grpc::Status status = stub_->CreateThreeDLinearActuator(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_dial(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteDialRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteDialResponse response;
    const grpc::Status status = stub_->DeleteDial(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_dial(const std::string& robot_name,
                            const std::string& part_name,
                            const std::string& dial_type,
                            const std::vector<std::string>& tags) {
    viam::app::v1::CreateDialRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_dial_type(dial_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateDialResponse response;
    const grpc::Status status = stub_->CreateDial(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_gauge(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteGaugeRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteGaugeResponse response;
    const grpc::Status status = stub_->DeleteGauge(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_gauge(const std::string& robot_name,
                             const std::string& part_name,
                             const std::string& gauge_type,
                             const std::vector<std::string>& tags) {
    viam::app::v1::CreateGaugeRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_gauge_type(gauge_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateGaugeResponse response;
    const grpc::Status status = stub_->CreateGauge(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_stop_watch(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteStopWatchRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteStopWatchResponse response;
    const grpc::Status status = stub_->DeleteStopWatch(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_stop_watch(const std::string& robot_name,
                                 const std::string& part_name,
                                 const std::string& stop_watch_type,
                                 const std::vector<std::string>& tags) {
    viam::app::v1::CreateStopWatchRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_stop_watch_type(stop_watch_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateStopWatchResponse response;
    const grpc::Status status = stub_->CreateStopWatch(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_time_of_flight_sensor(const std::string& robot_name,
                                            const std::string& part_name) {
    viam::app::v1::DeleteTimeOfFlightSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTimeOfFlightSensorResponse response;
    const grpc::Status status = stub_->DeleteTimeOfFlightSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_time_of_flight_sensor(const std::string& robot_name,
                                            const std::string& part_name,
                                            const std::string& time_of_flight_sensor_type,
                                            const std::vector<std::string>& tags) {
    viam::app::v1::CreateTimeOfFlightSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_time_of_flight_sensor_type(time_of_flight_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTimeOfFlightSensorResponse response;
    const grpc::Status status = stub_->CreateTimeOfFlightSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_camera(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotCameraRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotCameraResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotCamera(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_camera(const std::string& robot_name,
                                       const std::string& part_name,
                                       const std::string& turtlebot_camera_type,
                                       const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotCameraRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_camera_type(turtlebot_camera_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotCameraResponse response;
    const grpc::Status status = stub_->CreateTurtlebotCamera(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_grpc_camera(const std::string& robot_name,
                                            const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotGrpcCameraRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotGrpcCameraResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotGrpcCamera(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_grpc_camera(const std::string& robot_name,
                                             const std::string& part_name,
                                             const std::string& turtlebot_grpc_camera_type,
                                             const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotGrpcCameraRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_grpc_camera_type(turtlebot_grpc_camera_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotGrpcCameraResponse response;
    const grpc::Status status = stub_->CreateTurtlebotGrpcCamera(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_laser(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotLaserRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotLaserResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotLaser(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_laser(const std::string& robot_name,
                                      const std::string& part_name,
                                      const std::string& turtlebot_laser_type,
                                      const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotLaserRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_laser_type(turtlebot_laser_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotLaserResponse response;
    const grpc::Status status = stub_->CreateTurtlebotLaser(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_position_tracker(const std::string& robot_name,
                                                 const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotPositionTrackerRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotPositionTrackerResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotPositionTracker(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_position_tracker(const std::string& robot_name,
                                                 const std::string& part_name,
                                                 const std::string& turtlebot_position_tracker_type,
                                                 const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotPositionTrackerRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_position_tracker_type(turtlebot_position_tracker_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotPositionTrackerResponse response;
    const grpc::Status status = stub_->CreateTurtlebotPositionTracker(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheel(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel(const std::string& robot_name,
                                      const std::string& part_name,
                                      const std::string& turtlebot_wheel_type,
                                      const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_type(turtlebot_wheel_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheel(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_encoder(const std::string& robot_name,
                                             const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelEncoderRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelEncoderResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelEncoder(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_encoder(const std::string& robot_name,
                                              const std::string& part_name,
                                              const std::string& turtlebot_wheel_encoder_type,
                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelEncoderRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_encoder_type(turtlebot_wheel_encoder_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelEncoderResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelEncoder(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu(const std::string& robot_name, const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImu(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu(const std::string& robot_name,
                                          const std::string& part_name,
                                          const std::string& turtlebot_wheel_imu_type,
                                          const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_type(turtlebot_wheel_imu_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImu(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry(const std::string& robot_name,
                                               const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometry(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry(const std::string& robot_name,
                                               const std::string& part_name,
                                               const std::string& turtlebot_wheel_odometry_type,
                                               const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_type(turtlebot_wheel_odometry_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometry(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_sensor(const std::string& robot_name,
                                                 const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_sensor(const std::string& robot_name,
                                                 const std::string& part_name,
                                                 const std::string& turtlebot_wheel_imu_sensor_type,
                                                 const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_sensor_type(turtlebot_wheel_imu_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_sensor(const std::string& robot_name,
                                                      const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometrySensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometrySensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometrySensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_sensor(const std::string& robot_name,
                                                      const std::string& part_name,
                                                      const std::string& turtlebot_wheel_odometry_sensor_type,
                                                      const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometrySensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_sensor_type(turtlebot_wheel_odometry_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometrySensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometrySensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_encoder(const std::string& robot_name,
                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuEncoderRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuEncoderResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuEncoder(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_encoder(const std::string& robot_name,
                                                  const std::string& part_name,
                                                  const std::string& turtlebot_wheel_imu_encoder_type,
                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuEncoderRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_encoder_type(turtlebot_wheel_imu_encoder_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuEncoderResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuEncoder(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder(const std::string& robot_name,
                                                       const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoder(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder(const std::string& robot_name,
                                                       const std::string& part_name,
                                                       const std::string& turtlebot_wheel_odometry_encoder_type,
                                                       const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_type(turtlebot_wheel_odometry_encoder_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoder(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry(const std::string& robot_name,
                                                   const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometry(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry(const std::string& robot_name,
                                                   const std::string& part_name,
                                                   const std::string& turtlebot_wheel_imu_odometry_type,
                                                   const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_type(turtlebot_wheel_imu_odometry_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometry(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_encoder_sensor(const std::string& robot_name,
                                                         const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_encoder_sensor(const std::string& robot_name,
                                                         const std::string& part_name,
                                                         const std::string& turtlebot_wheel_imu_encoder_sensor_type,
                                                         const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_encoder_sensor_type(turtlebot_wheel_imu_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_sensor(const std::string& robot_name,
                                                           const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometrySensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometrySensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometrySensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_sensor(const std::string& robot_name,
                                                           const std::string& part_name,
                                                           const std::string& turtlebot_wheel_imu_odometry_sensor_type,
                                                           const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometrySensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_sensor_type(turtlebot_wheel_imu_odometry_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometrySensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometrySensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder(const std::string& robot_name,
                                                           const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoder(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder(const std::string& robot_name,
                                                           const std::string& part_name,
                                                           const std::string& turtlebot_wheel_imu_odometry_encoder_type,
                                                           const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_type(turtlebot_wheel_imu_odometry_encoder_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoder(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_imu_odometry_encoder_sensor_type(turtlebot_wheel_imu_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_odometry_encoder_sensor(const std::string& robot_name,
                                                              const std::string& part_name,
                                                              const std::string& turtlebot_wheel_odometry_encoder_sensor_type,
                                                              const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);
    request.set_turtlebot_wheel_odometry_encoder_sensor_type(turtlebot_wheel_odometry_encoder_sensor_type);
    for (const auto& tag : tags) {
        request.add_tags(tag);
    }

    grpc::ClientContext context;
    viam::app::v1::CreateTurtlebotWheelOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->CreateTurtlebotWheelOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::delete_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name) {
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_name(robot_name);
    request.set_part_name(part_name);

    grpc::ClientContext context;
    viam::app::v1::DeleteTurtlebotWheelImuOdometryEncoderSensorResponse response;
    const grpc::Status status = stub_->DeleteTurtlebotWheelImuOdometryEncoderSensor(&context, request, &response);

    if (!status.ok()) {
        throw RestAPIError(status.error_message(), status.error_code(), status.error_details());
    }
}

void AppClient::create_turtlebot_wheel_imu_odometry_encoder_sensor(const std::string& robot_name,
                                                                  const std::string& part_name,
                                                                  const std::string& turtlebot_wheel_imu_odometry_encoder_sensor_type,
                                                                  const std::vector<std::string>& tags) {
    viam::app::v1::CreateTurtlebotWheelImuOdometryEncoderSensorRequest request;
    request.set_robot_