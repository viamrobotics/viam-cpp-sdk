#include <viam/sdk/components/servo/server.hpp>

#include <viam/sdk/common/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/servo/servo.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
  namespace sdk {

    MotorServer::MotorServer() : ResourceServer(std::make_shared<ResourceManager>()){};
    MotorServer::MotorServer(std::shared_ptr<ResourceManager> manager) : ResourceServer(manager){};

    ::grpc::Status ServoServer::Move(
      ::grpc::ServerContext* context,
      const ::viam::component::servo::v1::MoveRequest* request,
      ::viam::component::servo::v1::MoveResponse* response) noexcept {
      return make_service_helper<Servo>(
        "ServoServer::Move", this, request)([&](auto& helper, auto& servo) {
          servo->move(request->angle_deg(), helper.getExtra());
      });
    }

    ::grpc::Status MotorServer::GetPosition(
      ::grpc::ServerContext* context,
      const ::viam::component::motor::v1::GetPositionRequest* request,
      ::viam::component::motor::v1::GetPositionResponse* response) noexcept {
      return make_service_helper<Motor>(
        "MotorServer::GetPosition", this, request)([&](auto& helper, auto& motor) {
        const Motor::position result = motor->get_position(helper.getExtra());
        response->set_position(result);
      });
    }

    ::grpc::Status MotorServer::Stop(::grpc::ServerContext* context,
                                    const ::viam::component::motor::v1::StopRequest* request,
                                    ::viam::component::motor::v1::StopResponse* response) noexcept {
      return make_service_helper<Motor>("MotorServer::Stop", this, request)(
        [&](auto& helper, auto& motor) { motor->stop(helper.getExtra()); });
    }


    ::grpc::Status MotorServer::IsMoving(
      ::grpc::ServerContext* context,
      const ::viam::component::motor::v1::IsMovingRequest* request,
      ::viam::component::motor::v1::IsMovingResponse* response) noexcept {
      return make_service_helper<Motor>(
        "MotorServer::IsMoving", this, request)([&](auto& helper, auto& motor) {
        const bool result = motor->is_moving();
        response->set_is_moving(result);
      });
    }

    ::grpc::Status MotorServer::GetGeometries(
      ::grpc::ServerContext* context,
      const ::viam::common::v1::GetGeometriesRequest* request,
      ::viam::common::v1::GetGeometriesResponse* response) noexcept {
      return make_service_helper<Motor>(
        "MotorServer::GetGeometries", this, request)([&](auto& helper, auto& motor) {
        const std::vector<GeometryConfig> geometries = motor->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = geometry.to_proto();
        }
      });
    }

    ::grpc::Status MotorServer::DoCommand(grpc::ServerContext* context,
                                          const viam::common::v1::DoCommandRequest* request,
                                          viam::common::v1::DoCommandResponse* response) noexcept {
      return make_service_helper<Motor>(
        "MotorServer::GetGeometries", this, request)([&](auto& helper, auto& motor) {
        const AttributeMap result = motor->do_command(struct_to_map(request->command()));
        *response->mutable_result() = map_to_struct(result);
      });
    }

    void MotorServer::register_server(std::shared_ptr<Server> server) {
        server->register_service(this);
    }

  }
}