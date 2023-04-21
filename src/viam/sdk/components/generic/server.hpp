/// @file components/generic/server.hpp
///
/// @brief Implements a gRPC server for the `Generic` component.
#pragma once

#include <viam/api/common/v1/common.grpc.pb.h>
#include <viam/api/component/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/resource/resource_server_base.hpp>
#include <viam/sdk/subtype/subtype.hpp>

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
    std::shared_ptr<SubtypeService> get_sub_svc();

    GenericServer() : sub_svc(std::make_shared<SubtypeService>()){};
    GenericServer(std::shared_ptr<SubtypeService> sub_svc) : sub_svc(sub_svc){};

   private:
    std::shared_ptr<SubtypeService> sub_svc;
};

}  // namespace sdk
}  // namespace viam