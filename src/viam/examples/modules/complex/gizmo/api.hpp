// Note that a `Gizmo` is implemented with `MyGizmo` in impl.hpp and impl.cpp.
#pragma once

#include <grpcpp/channel.h>
#include <vector>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>

#include "gizmo.grpc.pb.h"
#include "gizmo.pb.h"

using namespace viam::sdk;
using namespace viam::component::gizmo::v1;

// `GizmoRegistration` Defines a `ResourceRegistration` for the `Gizmo`
// component.
class GizmoRegistration : public ResourceRegistration {
   public:
    explicit GizmoRegistration(const google::protobuf::ServiceDescriptor* service_descriptor);
    std::shared_ptr<ResourceServer> create_resource_server(std::shared_ptr<ResourceManager> manager,
                                                           Server& server) override;
    std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                std::shared_ptr<grpc::Channel> chan) override;
};

// `Gizmo` is a custom modular component.
class Gizmo : public Component {
   public:
    // methods shared across all components
    static std::shared_ptr<ResourceRegistration> resource_registration();
    API api() const override;

    virtual bool do_one(std::string arg1) = 0;
    virtual bool do_one_client_stream(std::vector<std::string> arg1) = 0;
    virtual std::vector<bool> do_one_server_stream(std::string arg1) = 0;
    virtual std::vector<bool> do_one_bidi_stream(std::vector<std::string> arg1) = 0;
    virtual std::string do_two(bool arg1) = 0;

   protected:
    explicit Gizmo(std::string name);
};

namespace viam::sdk {
template <>
struct API::traits<Gizmo> {
    static ::viam::sdk::API api();
};
}  // namespace viam::sdk

// `GizmoClient` is the gRPC client implementation of a `Gizmo` component.
class GizmoClient : public Gizmo {
   public:
    GizmoClient(std::string name, std::shared_ptr<grpc::Channel> channel);

    bool do_one(std::string arg1) override;
    bool do_one_client_stream(std::vector<std::string> arg1) override;
    std::vector<bool> do_one_server_stream(std::string arg1) override;
    std::vector<bool> do_one_bidi_stream(std::vector<std::string> arg1) override;
    std::string do_two(bool arg1) override;

   private:
    using StubType = GizmoService::StubInterface;
    std::unique_ptr<StubType> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

// `GizmoServer` is the gRPC server implementation of a `Gizmo` component.
class GizmoServer : public ResourceServer, public GizmoService::Service {
   public:
    explicit GizmoServer(std::shared_ptr<ResourceManager> manager);

    grpc::Status DoOne(grpc::ServerContext* context,
                       const DoOneRequest* request,
                       DoOneResponse* response) override;

    grpc::Status DoOneClientStream(grpc::ServerContext* context,
                                   grpc::ServerReader<DoOneClientStreamRequest>* reader,
                                   DoOneClientStreamResponse* response) override;

    grpc::Status DoOneServerStream(grpc::ServerContext* context,
                                   const DoOneServerStreamRequest* request,
                                   grpc::ServerWriter<DoOneServerStreamResponse>* writer) override;

    grpc::Status DoOneBiDiStream(
        grpc::ServerContext* context,
        grpc::ServerReaderWriter<DoOneBiDiStreamResponse, DoOneBiDiStreamRequest>* stream) override;

    grpc::Status DoTwo(grpc::ServerContext* context,
                       const DoTwoRequest* request,
                       DoTwoResponse* response) override;
};
