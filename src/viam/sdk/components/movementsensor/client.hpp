/// @file components/movementsensor/client.hpp
///
/// @brief Implements a gRPC client for the `MovementSensor` component.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/movementsensor/v1/movementsensor.grpc.pb.h>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/components/movementsensor/movementsensor.hpp>
#include <viam/sdk/components/movementsensor/server.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

// todo move me
void throw_status(const ::grpc::Status& status);

/// @class MovementSensorClient
/// @brief gRPC client implementation of a `MovementSensor` component.
/// @ingroup MovementSensor
class MovementSensorClient : public MovementSensor {
   public:
    MovementSensorClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    Vector3 get_linear_velocity(const AttributeMap& extra) override;
    Vector3 get_angular_velocity(const AttributeMap& extra) override;
    compassheading get_compass_heading(const AttributeMap& extra) override;
    orientation get_orientation(const AttributeMap& extra) override;
    position get_position(const AttributeMap& extra) override;
    properties get_properties(const AttributeMap& extra) override;
    std::unordered_map<std::string, float> get_accuracy(const AttributeMap& extra) override;
    Vector3 get_linear_acceleration(const AttributeMap& extra) override;
    AttributeMap do_command(const AttributeMap& command) override;
    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;

    using MovementSensor::get_accuracy;
    using MovementSensor::get_angular_velocity;
    using MovementSensor::get_compass_heading;
    using MovementSensor::get_geometries;
    using MovementSensor::get_linear_acceleration;
    using MovementSensor::get_linear_velocity;
    using MovementSensor::get_orientation;
    using MovementSensor::get_position;
    using MovementSensor::get_properties;

    typedef viam::component::movementsensor::v1::MovementSensorService::StubInterface Stub;

   private:
    /// template to wrap a stub. todo: move to base class
    template <typename Request, typename Response, typename Cls>
    void stub_wrapper(Cls* self,
                      Response& resp,
                      const AttributeMap& extra,
                      ::grpc::Status (Cls::Stub::*method)(::grpc::ClientContext*,
                                                          const Request&,
                                                          Response*)) {
        Request request;
        grpc::ClientContext ctx;
        *request.mutable_name() = self->name();
        *request.mutable_extra() = map_to_struct(extra);
        throw_status((*(self->stub_).*method)(&ctx, request, &resp));
    }
    std::unique_ptr<Stub> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam
