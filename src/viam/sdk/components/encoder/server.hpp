/// @file components/encoder/server.hpp
///
/// @brief Implements a gRPC server for the `Encoder` component.
#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/encoder/v1/encoder.grpc.pb.h>

#include <viam/sdk/resource/resource_server_base.hpp>
#include <viam/sdk/subtype/subtype.hpp>

namespace viam {
namespace sdk {

/// @class EncoderServer
/// @brief gRPC server implementation of a `Encoder` component.
/// @ingroup Encoder
class EncoderServer : public ResourceServerBase,
                     public viam::component::encoder::v1::EncoderService::Service {
   public:
   
    ::grpc::Status GetPosition(::grpc::ServerContext* context,
                            const ::viam::component::encoder::v1::GetPositionRequest* request,
                            ::viam::component::encoder::v1::GetPositionResponse* response) override;
   
    ::grpc::Status ResetPosition(::grpc::ServerContext* context,
                            const ::viam::component::encoder::v1::ResetPositionRequest* request,
                            ::viam::component::encoder::v1::ResetPositionResponse* response) override;
   
    ::grpc::Status GetProperties(::grpc::ServerContext* context,
                            const ::viam::component::encoder::v1::GetPropertiesRequest* request,
                            ::viam::component::encoder::v1::GetPropertiesResponse* response) override;
   
    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                            const ::viam::component::encoder::v1::common.v1.DoCommandRequest* request,
                            ::viam::component::encoder::v1::common.v1.DoCommandResponse* response) override;
   

    void register_server(std::shared_ptr<Server> server) override;

    std::shared_ptr<SubtypeService> get_sub_svc();

    EncoderServer() : sub_svc(std::make_shared<SubtypeService>()){};
    EncoderServer(std::shared_ptr<SubtypeService> sub_svc) : sub_svc(sub_svc){};

   private:
    std::shared_ptr<SubtypeService> sub_svc;
};

}  // namespace sdk
}  // namespace viam
