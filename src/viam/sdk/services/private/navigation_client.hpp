/// @file services/navigation/client.hpp
///
/// @brief Implements a gRPC client for the `Navigation` service.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/service/navigation/v1/navigation.grpc.pb.h>

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
    NavigationClient(std::string name, std::shared_ptr<grpc::Channel> channel);

    Mode get_mode(const std::string name, const ProtoStruct& extra) override;
    void set_mode(const std::string name, const Mode mode, const ProtoStruct& extra) override;
    LocationResponse get_location(const std::string name, const ProtoStruct& extra) override;
    std::unique_ptr<std::vector<Waypoint>> get_waypoints(const std::string name,
                                                         const ProtoStruct& extra) override;
    void add_waypoint(const std::string name,
                      const geo_point& location,
                      const ProtoStruct& extra) override;
    void remove_waypoint(const std::string name,
                         const std::string id,
                         const ProtoStruct& extra) override;
    std::unique_ptr<std::vector<geo_geometry>> get_obstacles(const std::string name,
                                                             const ProtoStruct& extra) override;
    std::unique_ptr<std::vector<Path>> get_paths(const std::string name,
                                                 const ProtoStruct& extra) override;
    MapType get_properties(const std::string name) override;
    ProtoStruct do_command(const ProtoStruct& command) override;

   private:
    using StubType = service::navigation::v1::NavigationService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
