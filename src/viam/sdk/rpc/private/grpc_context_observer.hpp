#pragma once

#include <viam/sdk/common/grpc_fwd.hpp>

#include <viam/sdk/rpc/grpc_context_observer.hpp>

namespace viam {
namespace sdk {

class GrpcContextObserver::Enable {
   public:
    explicit Enable(const GrpcServerContext& context);
    ~Enable();
};

}  // namespace sdk
}  // namespace viam
