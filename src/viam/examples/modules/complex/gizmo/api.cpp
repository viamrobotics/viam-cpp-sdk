#include "api.hpp"

#include <vector>

#include <google/protobuf/descriptor.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

#include "../gen/gizmo.grpc.pb.h"
#include "../gen/gizmo.pb.h"

using namespace viam::sdk;
using namespace viam::component::gizmo::v1;

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
        p->FindServiceByName(GizmoService::service_full_name());
    if (!sd) {
        throw std::runtime_error("Unable to get service descriptor for the gizmo service");
    }
    return std::make_shared<GizmoRegistration>(sd);
}

API Gizmo::api() const {
    return API::for_t<Gizmo>();
}

Gizmo::Gizmo(std::string name) : Component(std::move(name)){};

/* Gizmo server methods */

GizmoServer::GizmoServer() : ResourceServer(std::make_shared<ResourceManager>()){};
GizmoServer::GizmoServer(std::shared_ptr<ResourceManager> manager) : ResourceServer(manager){};

grpc::Status GizmoServer::DoOne(grpc::ServerContext* context,
                                const DoOneRequest* request,
                                DoOneResponse* response) {
    if (!request) {
        return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                            "Called [Gizmo::DoOne] without a request");
    };

    auto rg = ResourceServer::resource_manager()->resource(request->name());
    if (!rg) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    // Downcast resource to Gizmo.
    const std::shared_ptr<Gizmo> gizmo = std::dynamic_pointer_cast<Gizmo>(rg);

    response->set_ret1(gizmo->do_one(request->arg1()));

    return grpc::Status();
}

grpc::Status GizmoServer::DoOneClientStream(grpc::ServerContext* context,
                                            grpc::ServerReader<DoOneClientStreamRequest>* reader,
                                            DoOneClientStreamResponse* response) {
    if (!reader) {
        return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                            "Called [Gizmo::DoOneClientStream] without a reader");
    };

    std::vector<std::string> args = {};
    std::string gizmo_name;
    DoOneClientStreamRequest curr_req;
    while (reader->Read(&curr_req)) {
        args.push_back(curr_req.arg1());
        if (!gizmo_name.empty() && curr_req.name() != gizmo_name) {
            return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                                "[Gizmo::DoOneClientStream] cannot reference multiple Gizmos");
        }
        gizmo_name = curr_req.name();
    }

    auto rg = ResourceServer::resource_manager()->resource(gizmo_name);
    if (!rg) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + gizmo_name);
    }

    // Downcast resource to Gizmo.
    const std::shared_ptr<Gizmo> gizmo = std::dynamic_pointer_cast<Gizmo>(rg);

    response->set_ret1(gizmo->do_one_client_stream(args));

    return grpc::Status();
}

grpc::Status GizmoServer::DoOneServerStream(grpc::ServerContext* context,
                                            const DoOneServerStreamRequest* request,
                                            grpc::ServerWriter<DoOneServerStreamResponse>* writer) {
    if (!request) {
        return grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                            "Called [Gizmo::DoOneServerStream] without a request");
    };

    auto rg = ResourceServer::resource_manager()->resource(request->name());
    if (!rg) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    // Downcast resource to Gizmo.
    const std::shared_ptr<Gizmo> gizmo = std::dynamic_pointer_cast<Gizmo>(rg);

    for (bool ret1 : gizmo->do_one_server_stream(request->arg1())) {
        DoOneServerStreamResponse curr_resp = {};
        curr_resp.set_ret1(ret1);
        writer->Write(curr_resp);
    }

    return grpc::Status();
}

grpc::Status GizmoServer::DoOneBiDiStream(
    grpc::ServerContext* context,
    grpc::ServerReaderWriter<DoOneBiDiStreamResponse, DoOneBiDiStreamRequest>* stream) {
    if (!stream) {
        return grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                            "Called [Gizmo::DoOneBiDiStream] without a stream");
    };

    std::vector<std::string> args = {};
    std::string gizmo_name;
    DoOneBiDiStreamRequest curr_req;
    while (stream->Read(&curr_req)) {
        args.push_back(curr_req.arg1());
        if (!gizmo_name.empty() && curr_req.name() != gizmo_name) {
            return grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                                "[Gizmo::DoOneBiDiStream] cannot reference multiple Gizmos");
        }
        gizmo_name = curr_req.name();
    }

    auto rg = ResourceServer::resource_manager()->resource(gizmo_name);
    if (!rg) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + gizmo_name);
    }

    // Downcast resource to Gizmo.
    const std::shared_ptr<Gizmo> gizmo = std::dynamic_pointer_cast<Gizmo>(rg);

    for (bool ret1 : gizmo->do_one_bidi_stream(args)) {
        DoOneBiDiStreamResponse curr_resp = {};
        curr_resp.set_ret1(ret1);
        stream->Write(curr_resp);
    }
    return grpc::Status();
}

grpc::Status GizmoServer::DoTwo(::grpc::ServerContext* context,
                                const DoTwoRequest* request,
                                DoTwoResponse* response) {
    if (!request) {
        return grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                            "Called [Gizmo::DoTwo] without a request");
    };

    auto rg = ResourceServer::resource_manager()->resource(request->name());
    if (!rg) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    // Downcast resource to Gizmo.
    const std::shared_ptr<Gizmo> gizmo = std::dynamic_pointer_cast<Gizmo>(rg);

    response->set_ret1(gizmo->do_two(request->arg1()));

    return grpc::Status();
}

void GizmoServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

/* Gizmo client methods */

GizmoClient::GizmoClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Gizmo(std::move(name)), stub_(GizmoService::NewStub(channel)), channel_(std::move(channel)){};

bool GizmoClient::do_one(std::string arg1) {
    return make_client_helper(this, *stub_, &StubType::DoOne)
        .with([&](auto& request) { request.set_arg1(arg1); })
        .invoke([](auto& response) { return response.ret1(); });
}

bool GizmoClient::do_one_client_stream(std::vector<std::string> arg1) {
    DoOneClientStreamResponse response;
    ClientContext ctx;

    auto writer(stub_->DoOneClientStream(ctx, &response));
    for (std::string arg : arg1) {
        DoOneClientStreamRequest curr_req = {};
        *curr_req.mutable_name() = this->name();
        curr_req.set_arg1(arg);
        if (!writer->Write(curr_req)) {
            // Stream is broken; stop writing.
            break;
        }
    }
    writer->WritesDone();
    grpc::Status status = writer->Finish();
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }

    return response.ret1();
}

std::vector<bool> GizmoClient::do_one_server_stream(std::string arg1) {
    DoOneServerStreamRequest request;
    ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_arg1(arg1);

    auto reader(stub_->DoOneServerStream(ctx, request));
    DoOneServerStreamResponse curr_resp = {};
    std::vector<bool> rets = {};
    while (reader->Read(&curr_resp)) {
        rets.push_back(curr_resp.ret1());
    }
    grpc::Status status = reader->Finish();
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }

    return rets;
}

std::vector<bool> GizmoClient::do_one_bidi_stream(std::vector<std::string> arg1) {
    ClientContext ctx;

    auto stream(stub_->DoOneBiDiStream(ctx));
    for (std::string arg : arg1) {
        DoOneBiDiStreamRequest curr_req = {};
        *curr_req.mutable_name() = this->name();
        curr_req.set_arg1(arg);
        if (!stream->Write(curr_req)) {
            // Stream is broken; stop writing.
            break;
        }
    }
    stream->WritesDone();

    DoOneBiDiStreamResponse curr_resp = {};
    std::vector<bool> rets = {};
    while (stream->Read(&curr_resp)) {
        rets.push_back(curr_resp.ret1());
    }

    grpc::Status status = stream->Finish();
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }

    return rets;
}

std::string GizmoClient::do_two(bool arg1) {
    DoTwoRequest request;
    DoTwoResponse response;

    ClientContext ctx;

    *request.mutable_name() = this->name();
    request.set_arg1(arg1);

    const grpc::Status status = stub_->DoTwo(ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }

    return response.ret1();
}
