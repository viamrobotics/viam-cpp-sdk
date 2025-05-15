/// @file services/generic/client.hpp
///
/// @brief Implements a gRPC client for the `GenericService`.
#pragma once

#include <viam/api/service/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/services/generic.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class GenericServiceClient
/// @brief gRPC client implementation of a `GenericService`.
/// @ingroup GenericService
class GenericServiceClient : public GenericService {
   public:
    using interface_type = GenericService;
    GenericServiceClient(std::string name, const ViamChannel& channel);

    const ViamChannel& channel() const {
        return *channel_;
    }

    ProtoStruct do_command(const ProtoStruct& command) override;

   protected:
    // This constructor leaves the `channel_` as a nullptr. This is useful for testing
    // purposes, but renders it unusable for production use. Care should be taken to
    // avoid use of this constructor outside of tests.
    GenericServiceClient(
        std::string name,
        std::unique_ptr<viam::service::generic::v1::GenericService::StubInterface> stub)
        : GenericService(std::move(name)), stub_(std::move(stub)) {}

   private:
    using StubType = viam::service::generic::v1::GenericService::StubInterface;
    std::unique_ptr<StubType> stub_;
    const ViamChannel* channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
