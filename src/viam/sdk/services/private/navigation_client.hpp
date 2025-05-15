/// @file services/navigation/client.hpp
///
/// @brief Implements a gRPC client for the `Navigation` service.
#pragma once

#include <viam/api/service/navigation/v1/navigation.grpc.pb.h>

#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/services/navigation.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class NavigationClient
/// @brief gRPC client implementation of a `Navigation` service.
/// @ingroup Navigation
class NavigationClient : public Navigation {
   public:
    using interface_type = Navigation;
    NavigationClient(std::string name, ViamChannel& channel);

    const ViamChannel& channel() const {
        return *channel_;
    }

    Mode get_mode(const ProtoStruct& extra) override;
    void set_mode(const Mode mode, const ProtoStruct& extra) override;
    LocationResponse get_location(const ProtoStruct& extra) override;
    std::vector<Waypoint> get_waypoints(const ProtoStruct& extra) override;
    void add_waypoint(const geo_point& location, const ProtoStruct& extra) override;
    void remove_waypoint(const std::string id, const ProtoStruct& extra) override;
    std::vector<geo_geometry> get_obstacles(const ProtoStruct& extra) override;
    std::vector<Path> get_paths(const ProtoStruct& extra) override;
    Properties get_properties() override;
    ProtoStruct do_command(const ProtoStruct& command) override;

   private:
    using StubType = service::navigation::v1::NavigationService::StubInterface;
    std::unique_ptr<StubType> stub_;
    ViamChannel* channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
