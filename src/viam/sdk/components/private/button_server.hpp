/// @file components/private/button_server.hpp
///
/// @brief Implements a gRPC server for the `Button` component
#pragma once

#include <viam/api/component/button/v1/button.grpc.pb.h>
#include <viam/api/component/button/v1/button.pb.h>

#include <viam/sdk/components/button.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {
namespace impl {

class ButtonServer : public ResourceServer,
                     public viam::component::button::v1::ButtonService::Service {
   public:
    using interface_type = Button;
    using service_type = component::button::v1::ButtonService;

    explicit ButtonServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status Push(::grpc::ServerContext* context,
                        const ::viam::component::button::v1::PushRequest* request,
                        ::viam::component::button::v1::PushResponse* response) noexcept override;

    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) noexcept override;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
