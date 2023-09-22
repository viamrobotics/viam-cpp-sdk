/// @file complex/summation/api.hpp
///
/// @brief Defines a `Summation` service.
///
/// Note that `Summation` is implemented with `MySummation` in impl.hpp and impl.cpp.
#pragma once

#include <vector>

#include <grpcpp/channel.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>
#include <viam/sdk/services/service.hpp>

#include "../proto/summation.grpc.pb.h"
#include "../proto/summation.pb.h"

using namespace viam::sdk;
using namespace viam::service::summation::v1;

/// @defgroup Summation Classes related to the Summation service.

/// @class SummationRegistration
/// @brief Defines a `ResourceRegistration` for the `Summation` service.
/// @ingroup Summation
class SummationRegistration : public ResourceRegistration {
   public:
    explicit SummationRegistration(const google::protobuf::ServiceDescriptor* service_descriptor);
    std::shared_ptr<ResourceServer> create_resource_server(
        std::shared_ptr<ResourceManager> manager) override;
    std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                std::shared_ptr<grpc::Channel> chan) override;
};

/// @class Summation api.hpp "complex/summation/api.hpp"
/// @brief A `Summation` is a custom modular service.
/// @ingroup Summation
class Summation : public Service {
   public:
    // methods shared across all services
    static std::shared_ptr<ResourceRegistration> resource_registration();
    static API static_api();
    API dynamic_api() const override;

    virtual double sum(std::vector<double> numbers) = 0;

   protected:
    explicit Summation(std::string name);
};

/// @class SummationClient
/// @brief gRPC client implementation of a `Summation` service.
/// @ingroup Summation
class SummationClient : public Summation {
   public:
    SummationClient(std::string name, std::shared_ptr<grpc::Channel> channel);

    double sum(std::vector<double> numbers) override;

   private:
    std::unique_ptr<SummationService::StubInterface> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

/// @class SummationServer
/// @brief gRPC server implementation of a `Summation` service.
/// @ingroup Summation
class SummationServer : public ResourceServer, public SummationService::Service {
   public:
    SummationServer();
    explicit SummationServer(std::shared_ptr<ResourceManager> manager);

    grpc::Status Sum(grpc::ServerContext* context,
                     const SumRequest* request,
                     SumResponse* response) override;

    void register_server(std::shared_ptr<Server> server) override;
};
