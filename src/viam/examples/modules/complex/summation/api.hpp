// Note that `Summation` is implemented with `MySummation` in impl.hpp and impl.cpp.
#pragma once

#include <vector>

#include <grpcpp/channel.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/services/service.hpp>

#include "summation.grpc.pb.h"
#include "summation.pb.h"

using namespace viam::sdk;
using namespace viam::service::summation::v1;

// A `Summation` is a custom modular service.
class Summation : public Service {
   public:
    // methods shared across all services
    API api() const override;

    virtual double sum(std::vector<double> numbers) = 0;

   protected:
    explicit Summation(std::string name);
};

namespace viam {
namespace sdk {
template <>
struct API::traits<Summation> {
    static API api();
};
}  // namespace sdk
}  // namespace viam

// `SummationClient` is the gRPC client implementation of a `Summation`
// service.
class SummationClient : public Summation {
   public:
    using interface_type = Summation;
    SummationClient(std::string name, const ViamChannel& channel);

    const ViamChannel& channel() const {
        return *channel_;
    }

    double sum(std::vector<double> numbers) override;

   private:
    using StubType = SummationService::StubInterface;
    std::unique_ptr<StubType> stub_;
    const ViamChannel* channel_;
};

// `SummationServer` is the gRPC server implementation of a `Summation`
// service.
class SummationServer : public ResourceServer, public SummationService::Service {
   public:
    using interface_type = Summation;
    using service_type = SummationService;
    explicit SummationServer(std::shared_ptr<ResourceManager> manager);

    grpc::Status Sum(grpc::ServerContext* context,
                     const SumRequest* request,
                     SumResponse* response) override;
};
