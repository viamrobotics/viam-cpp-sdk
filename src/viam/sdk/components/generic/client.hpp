/// @file components/generic/client.hpp
///
/// @brief Implements a gRPC client for the `GenericComponent`.
#pragma once

#include <grpcpp/channel.h>

#include <viam/api/component/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/components/generic/generic.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

/// @class GenericComponentClient
/// @brief gRPC client implementation of a `GenericComponent`.
/// @ingroup GenericComponent
class GenericComponentClient : public GenericComponent {
   public:
    GenericComponentClient(std::string name, std::shared_ptr<grpc::Channel> channel);
    AttributeMap do_command(AttributeMap command) override;
    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;

   protected:
    // This constructor leaves the `channel_` as a nullptr. This is useful for testing
    // purposes, but renders it unusable for production use. Care should be taken to
    // avoid use of this constructor outside of tests.
    GenericComponentClient(
        std::string name,
        std::unique_ptr<viam::component::generic::v1::GenericService::StubInterface> stub)
        : GenericComponent(std::move(name)), stub_(std::move(stub)){};

   private:
    using StubType = viam::component::generic::v1::GenericService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

}  // namespace sdk
}  // namespace viam
