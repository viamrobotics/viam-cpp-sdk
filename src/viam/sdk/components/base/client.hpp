/// @file components/base/client.hpp
///
/// @brief Implements a gRPC client for the `Base` component.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/base/v1/base.grpc.pb.h>

#include <viam/sdk/common/linear_algebra.hpp>
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
    void move_straight(int64_t distance_mm, double mm_per_sec) override;
    void spin(double angle_deg, double degs_per_sec) override;
    void set_power(Vector3 linear, Vector3 angular) override;
    void set_velocity(Vector3 linear, Vector3 angular) override;
    grpc::StatusCode stop() override;
    bool is_moving() override;
    AttributeMap do_command(AttributeMap command) override;
    BaseClient(std::string name, std::shared_ptr<grpc::Channel> channel)
        : Base(std::move(name)),
          stub_(viam::component::base::v1::BaseService::NewStub(channel)),
          channel_(std::move(channel)){};

   private:
    std::unique_ptr<viam::component::base::v1::BaseService::StubInterface> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam
