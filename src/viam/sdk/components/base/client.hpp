/// @file components/base/client.hpp
///
/// @brief Implements a gRPC client for the `Base` component.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/base/v1/base.grpc.pb.h>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/base/base.hpp>
#include <viam/sdk/components/base/server.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

/// @class BaseClient
/// @brief gRPC client implementation of a `Base` component.
/// @ingroup Base
class BaseClient : public Base {
   public:
    BaseClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    void move_straight(int64_t distance_mm, double mm_per_sec) override;
    void move_straight(int64_t distance_mm, double mm_per_sec, const AttributeMap& extra) override;
    void spin(double angle_deg, double degs_per_sec) override;
    void spin(double angle_deg, double degs_per_sec, const AttributeMap& extra) override;
    void set_power(const Vector3& linear, const Vector3& angular) override;
    void set_power(const Vector3& linear,
                   const Vector3& angular,
                   const AttributeMap& extra) override;
    void set_velocity(const Vector3& linear, const Vector3& angular) override;
    void set_velocity(const Vector3& linear,
                      const Vector3& angular,
                      const AttributeMap& extra) override;
    grpc::StatusCode stop() override;
    grpc::StatusCode stop(const AttributeMap& extra) override;
    bool is_moving() override;
    AttributeMap do_command(const AttributeMap& command) override;

   private:
    std::unique_ptr<viam::component::base::v1::BaseService::StubInterface> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam
