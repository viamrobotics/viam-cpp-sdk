/// @file complex/gizmo/api.hpp
///
/// @brief Defines a `Gizmo` component.
///
/// Note that `Gizmo` is implemented with `MyGizmo` in impl.hpp and impl.cpp.
#pragma once

#include <grpcpp/channel.h>
#include <vector>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>

#include "../proto/gizmo.grpc.pb.h"

using namespace viam::sdk;

/// @defgroup Gizmo Classes related to the Gizmo component.

/// @class GizmoRegistration
/// @brief Defines a `ResourceRegistration` for the `Gizmo` component.
/// @ingroup Gizmo
class GizmoRegistration : public ResourceRegistration {
   public:
    explicit GizmoRegistration(const google::protobuf::ServiceDescriptor* service_descriptor);
    std::shared_ptr<ResourceServer> create_resource_server(
        std::shared_ptr<ResourceManager> manager) override;
    std::shared_ptr<Resource> create_rpc_client(std::string name,
                                                std::shared_ptr<grpc::Channel> chan) override;
};

/// @class Gizmo api.hpp "complex/gizmo/api.hpp"
/// @brief A `Gizmo` is a custom modular component.
/// @ingroup Gizmo
class Gizmo : public Component {
   public:
    // methods shared across all components
    static std::shared_ptr<ResourceRegistration> resource_registration();
    static API static_api();
    API dynamic_api() const override;

    virtual bool do_one(std::string arg1) = 0;
    virtual bool do_one_client_stream(std::vector<std::string> arg1) = 0;
    virtual std::vector<bool> do_one_server_stream(std::string arg1) = 0;
    virtual std::vector<bool> do_one_bidi_stream(std::vector<std::string> arg1) = 0;
    virtual std::string do_two(bool arg1) = 0;

   protected:
    explicit Gizmo(std::string name);
};

/// @class GizmoClient
/// @brief gRPC client implementation of a `Gizmo` component.
/// @ingroup Gizmo
class GizmoClient : public Gizmo {
   public:
    GizmoClient(std::string name, std::shared_ptr<grpc::Channel> channel);

    bool do_one(std::string arg1) override;
    bool do_one_client_stream(std::vector<std::string> arg1) override;
    std::vector<bool> do_one_server_stream(std::string arg1) override;
    std::vector<bool> do_one_bidi_stream(std::vector<std::string> arg1) override;
    std::string do_two(bool arg1) override;

   private:
    std::unique_ptr<viam::component::gizmo::v1::GizmoService::StubInterface> stub_;
    std::shared_ptr<grpc::Channel> channel_;
};

/// @class GizmoServer
/// @brief gRPC server implementation of a `Gizmo` component.
/// @ingroup Gizmo
class GizmoServer : public ResourceServer,
                    public viam::component::gizmo::v1::GizmoService::Service {
   public:
    GizmoServer();
    explicit GizmoServer(std::shared_ptr<ResourceManager> manager);

    ::grpc::Status DoOne(::grpc::ServerContext* context,
                         const ::viam::component::gizmo::v1::DoOneRequest* request,
                         ::viam::component::gizmo::v1::DoOneResponse* response) override;

    ::grpc::Status DoOneClientStream(
        ::grpc::ServerContext* context,
        ::grpc::ServerReader<::viam::component::gizmo::v1::DoOneClientStreamRequest>* reader,
        ::viam::component::gizmo::v1::DoOneClientStreamResponse* response) override;

    ::grpc::Status DoOneServerStream(
        ::grpc::ServerContext* context,
        const ::viam::component::gizmo::v1::DoOneServerStreamRequest* request,
        ::grpc::ServerWriter<::viam::component::gizmo::v1::DoOneServerStreamResponse>* writer)
        override;

    ::grpc::Status DoOneBiDiStream(
        ::grpc::ServerContext* context,
        ::grpc::ServerReaderWriter<::viam::component::gizmo::v1::DoOneBiDiStreamResponse,
                                   ::viam::component::gizmo::v1::DoOneBiDiStreamRequest>* stream)
        override;

    ::grpc::Status DoTwo(::grpc::ServerContext* context,
                         const ::viam::component::gizmo::v1::DoTwoRequest* request,
                         ::viam::component::gizmo::v1::DoTwoResponse* response) override;

    void register_server(std::shared_ptr<Server> server) override;
};
