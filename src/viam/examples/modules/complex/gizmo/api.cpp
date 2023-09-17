#include <vector>

#include <google/protobuf/descriptor.h>

#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

#include "api.hpp"

using namespace viam::sdk;

/* GizmoRegistration methods */

GizmoRegistration::GizmoRegistration(const google::protobuf::ServiceDescriptor* service_descriptor)
    : ResourceRegistration(service_descriptor){};

std::shared_ptr<ResourceServer> GizmoRegistration::create_resource_server(
    std::shared_ptr<ResourceManager> manager) {
    return std::make_shared<GizmoServer>(manager);
};

std::shared_ptr<Resource> GizmoRegistration::create_rpc_client(
    std::string name, std::shared_ptr<grpc::Channel> chan) {
    return std::make_shared<GizmoClient>(std::move(name), std::move(chan));
};

/* Gizmo methods */

std::shared_ptr<ResourceRegistration> Gizmo::resource_registration() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::gizmo::v1::GizmoService::service_full_name());
    if (!sd) {
        throw std::runtime_error("Unable to get service descriptor for the gizmo service");
    }
    return std::make_shared<GizmoRegistration>(sd);
}

API Gizmo::static_api() {
    return {"viam", "gizmo", "base"};
}

API Gizmo::dynamic_api() const {
    return static_api();
}

Gizmo::Gizmo(std::string name) : Component(std::move(name)){};

/* Gizmo server methods */

GizmoServer::GizmoServer(std::shared_ptr<ResourceManager> manager) : ResourceServer(manager){};

::grpc::Status GizmoServer::DoOne(::grpc::ServerContext* context,
                                  const ::viam::component::gizmo::v1::DoOneRequest* request,
                                  ::viam::component::gizmo::v1::DoOneResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Gizmo::DoOne] without a request");
    };

    auto rg = ResourceServer::resource_manager()->resource(request->name());
    if (!rg) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Gizmo> gizmo = std::dynamic_pointer_cast<Gizmo>(rg);

    gizmo->do_one(request->arg1());

    // TODO(benji) pack the response into &response.

    return ::grpc::Status();
}

::grpc::Status GizmoServer::DoOneClientStream(
    ::grpc::ServerContext* context,
    ::grpc::ServerReader<::viam::component::gizmo::v1::DoOneClientStreamRequest>* reader,
    ::viam::component::gizmo::v1::DoOneClientStreamResponse* response) {
    if (!reader) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Gizmo::DoOneClientStream] without a reader");
    };

    std::vector<std::string> args = {};
    std::string gizmo_name;
    viam::component::gizmo::v1::DoOneClientStreamRequest curr_req;
    while (reader->Read(&curr_req)) {
        args.push_back(curr_req.arg1());
        if (!gizmo_name.empty() && curr_req.name() != gizmo_name) {
            return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                                  "[Gizmo::DoOneClientStream] cannot reference multiple Gizmos");
        }
        gizmo_name = curr_req.name();
    }

    auto rg = ResourceServer::resource_manager()->resource(gizmo_name);
    if (!rg) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + gizmo_name);
    }

    const std::shared_ptr<Gizmo> gizmo = std::dynamic_pointer_cast<Gizmo>(rg);

    auto resp = gizmo->do_one_client_stream(args);

    // TODO(benji) pack the response into &response.

    return ::grpc::Status();
}

::grpc::Status GizmoServer::DoOneServerStream(
    ::grpc::ServerContext* context,
    const ::viam::component::gizmo::v1::DoOneServerStreamRequest* request,
    ::grpc::ServerWriter<::viam::component::gizmo::v1::DoOneServerStreamResponse>* writer) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Gizmo::DoOneServerStream] without a request");
    };

    // TODO(benji): Implement streaming gizmo things.

    return ::grpc::Status();
}

::grpc::Status GizmoServer::DoOneBiDiStream(
    ::grpc::ServerContext* context,
    ::grpc::ServerReaderWriter<::viam::component::gizmo::v1::DoOneBiDiStreamResponse,

                               ::viam::component::gizmo::v1::DoOneBiDiStreamRequest>* stream) {
    if (!stream) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Gizmo::DoOneServerStream] without a stream");
    };

    // TODO(benji): Implement streaming gizmo things.

    return ::grpc::Status();
}

::grpc::Status GizmoServer::DoTwo(::grpc::ServerContext* context,
                                  const ::viam::component::gizmo::v1::DoTwoRequest* request,
                                  ::viam::component::gizmo::v1::DoTwoResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Gizmo::DoTwo] without a request");
    };

    auto rb = ResourceServer::resource_manager()->resource(request->name());
    if (!rb) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    const std::shared_ptr<Gizmo> gizmo = std::dynamic_pointer_cast<Gizmo>(rb);

    gizmo->do_two(request->arg1());

    // TODO(benji) pack the response into &response.

    return ::grpc::Status();
}

void GizmoServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

/* Gizmo client methods */

GizmoClient::GizmoClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Gizmo(std::move(name)),
      stub_(viam::component::gizmo::v1::GizmoService::NewStub(channel)),
      channel_(std::move(channel)){};

bool GizmoClient::do_one(std::string arg1) {
    viam::component::gizmo::v1::DoOneRequest request;
    viam::component::gizmo::v1::DoOneResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_arg1(arg1);

    const grpc::Status status = stub_->DoOne(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }

    return response.ret1();
}

bool GizmoClient::do_one_client_stream(std::vector<std::string> arg1) {
    // TODO(benji): implement these streaming gizmo things.
}

std::vector<bool> GizmoClient::do_one_server_stream(std::string arg1) {
    // TODO(benji): implement these streaming gizmo things.
}

std::vector<bool> GizmoClient::do_one_bidi_stream(std::vector<std::string> arg1) {
    // TODO(benji): implement these streaming gizmo things.
}

std::string GizmoClient::do_two(bool arg1) {
    viam::component::gizmo::v1::DoTwoRequest request;
    viam::component::gizmo::v1::DoTwoResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_arg1(arg1);

    const grpc::Status status = stub_->DoTwo(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }

    return response.ret1();
}

/* Gizmo registration logic. Automatically registers the Gizmo API upon including
 * this header file. */

bool init() {
    Registry::register_resource(Gizmo::static_api(), Gizmo::resource_registration());
    return true;
};

// NOLINTNEXTLINE
const bool inited = init();
