/// @file components/private/base_client.hpp
///
/// @brief Implements a gRPC client for the `Base` component.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/base/v1/base.grpc.pb.h>

#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/components/base.hpp>
#include <viam/sdk/components/private/base_server.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class BaseClient
/// @brief gRPC client implementation of a `Base` component.
/// @ingroup Base
class BaseClient : public Base {
   public:
    using interface_type = Base;
    BaseClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    void move_straight(int64_t distance_mm, double mm_per_sec, const AttributeMap& extra) override;
    void spin(double angle_deg, double degs_per_sec, const AttributeMap& extra) override;
    void set_power(const Vector3& linear,
                   const Vector3& angular,
                   const AttributeMap& extra) override;
    void set_velocity(const Vector3& linear,
                      const Vector3& angular,
                      const AttributeMap& extra) override;
    void stop(const AttributeMap& extra) override;
    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;
    properties get_properties(const AttributeMap& extra) override;
    bool is_moving() override;
    AttributeMap do_command(const AttributeMap& command) override;

    // the `extra` param is frequently unnecessary but needs to be supported. Ideally, we'd
    // like to live in a world where implementers of derived classes don't need to go out of
    // their way to support two versions of a method (an `extra` version and a non-`extra`
    // version), and users don't need to pass an unnecessary parameters to all method calls.
    //
    // To do this, we define in the parent resource class a non-virtual version of the methods
    // that calls the virtual method and passes a `nullptr` by default in place of the `extra`
    // param. In order to access these versions of the methods within the client code, however,
    // we need to include these `using` lines.
    using Base::get_geometries;
    using Base::get_properties;
    using Base::move_straight;
    using Base::set_power;
    using Base::set_velocity;
    using Base::spin;
    using Base::stop;

   private:
    using StubType = viam::component::base::v1::BaseService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
