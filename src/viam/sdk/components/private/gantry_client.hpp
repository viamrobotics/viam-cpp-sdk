/// @file components/private/gantry_client.hpp
///
/// @brief Implements a gRPC client for the `Gantry` component
#pragma once

#include <viam/api/component/gantry/v1/gantry.grpc.pb.h>

#include <viam/sdk/components/gantry.hpp>
#include <viam/sdk/rpc/dial.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class GantryClient
/// @brief gRPC client implementation of a `Gantry` component.
/// @ingroup Gantry
class GantryClient : public Gantry {
   public:
    using interface_type = Gantry;
    GantryClient(std::string name, const ViamChannel& channel);

    const ViamChannel& channel() const {
        return *channel_;
    }

    std::vector<double> get_position(const ProtoStruct& extra) override;
    void move_to_position(const std::vector<movement_coordinate>& coordinates,
                          const ProtoStruct& extra) override;
    bool home(const ProtoStruct& extra) override;
    std::vector<double> get_lengths(const ProtoStruct& extra) override;
    bool is_moving() override;
    void stop(const ProtoStruct& extra) override;
    ProtoStruct do_command(const ProtoStruct& command) override;
    std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) override;
    Gantry::KinematicsData get_kinematics(const ProtoStruct& extra) override;

    using Gantry::get_geometries;
    using Gantry::get_kinematics;
    using Gantry::get_lengths;
    using Gantry::get_position;
    using Gantry::home;
    using Gantry::move_to_position;
    using Gantry::stop;

   private:
    using StubType = viam::component::gantry::v1::GantryService::StubInterface;
    std::unique_ptr<StubType> stub_;
    const ViamChannel* channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
