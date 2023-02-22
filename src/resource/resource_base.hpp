#pragma once
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>

#include <common/proto_type.hpp>
#include <unordered_map>

class ResourceBase : public grpc::Service {
   public:
    virtual grpc::StatusCode stop(std::unordered_map<std::string, ProtoType*> extra);
    virtual grpc::StatusCode stop();
};
