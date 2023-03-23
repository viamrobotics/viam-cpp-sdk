#pragma once

#include <common/v1/common.grpc.pb.h>
#include <component/generic/v1/generic.grpc.pb.h>

#include <resource/resource_server_base.hpp>
#include <subtype/subtype.hpp>

class GenericServer : public ResourceServerBase,
                      public viam::component::generic::v1::GenericService::Service {
   public:
    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::common::v1::DoCommandRequest* request,
                             ::viam::common::v1::DoCommandResponse* response) override;

    void register_server() override;

    GenericServer(){};
    GenericServer(std::shared_ptr<SubtypeService> sub_svc) : sub_svc(sub_svc){};

   private:
    std::shared_ptr<SubtypeService> sub_svc;
};
