// CR erodkin: do we need anything else in here?

#include <grpcpp/support/status.h>

#include <common/proto_type.hpp>
#include <unordered_map>

class ResourceBase {
    virtual grpc::StatusCode stop(std::unordered_map<std::string, ProtoType*> extra);
    virtual grpc::StatusCode stop();
};

grpc::StatusCode ResourceBase::stop(std::unordered_map<std::string, ProtoType*> ex) {
    return stop();
}

grpc::StatusCode ResourceBase::stop() {
    return grpc::StatusCode::UNIMPLEMENTED;
}

