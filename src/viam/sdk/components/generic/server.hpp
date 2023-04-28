/// @file components/generic/server.hpp
///
/// @brief Implements a gRPC server for the `Generic` component.
#pragma once

#include <viam/api/common/v1/common.grpc.pb.h>
#include <viam/api/component/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {

/// @class GenericServer
/// @brief gRPC server implementation of a `Generic` component.
/// @ingroup Generic
class GenericServer : public ResourceServerBase,
                      public viam::component::generic::v1::GenericService::Service {
   public:
    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) override;

    void register_server(std::shared_ptr<Server> server) override;

    GenericServer() : ResourceServerBase(std::make_shared<ResourceManager>()){};
    GenericServer(std::shared_ptr<ResourceManager> manager) : ResourceServerBase(manager){};
};

}  // namespace sdk
}  // namespace viam
