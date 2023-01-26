#pragma once
#include <grpcpp/support/status.h>

#include <common/proto_type.hpp>
#include <unordered_map>

class ResourceBase {
   public:
    virtual grpc::StatusCode stop(std::unordered_map<std::string, ProtoType*> extra);
    virtual grpc::StatusCode stop();
};
