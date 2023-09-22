#include <vector>

#include <google/protobuf/descriptor.h>

#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

#include "../proto/summation.grpc.pb.h"
#include "../proto/summation.pb.h"
#include "api.hpp"

using namespace viam::sdk;
using namespace viam::service::summation::v1;

/* SummationRegistration methods */

SummationRegistration::SummationRegistration(
    const google::protobuf::ServiceDescriptor* service_descriptor)
    : ResourceRegistration(service_descriptor){};

std::shared_ptr<ResourceServer> SummationRegistration::create_resource_server(
    std::shared_ptr<ResourceManager> manager) {
    return std::make_shared<SummationServer>(manager);
};

std::shared_ptr<Resource> SummationRegistration::create_rpc_client(
    std::string name, std::shared_ptr<grpc::Channel> chan) {
    return std::make_shared<SummationClient>(std::move(name), std::move(chan));
};

/* Summation methods */

std::shared_ptr<ResourceRegistration> Summation::resource_registration() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(SummationService::service_full_name());
    if (!sd) {
        throw std::runtime_error("Unable to get service descriptor for the summation service");
    }
    return std::make_shared<SummationRegistration>(sd);
}

API Summation::static_api() {
    return {"viam", "service", "summation"};
}

API Summation::dynamic_api() const {
    return static_api();
}

Summation::Summation(std::string name) : Service(std::move(name)){};

/* Summation server methods */

SummationServer::SummationServer() : ResourceServer(std::make_shared<ResourceManager>()){};
SummationServer::SummationServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(manager){};

grpc::Status SummationServer::Sum(grpc::ServerContext* context,
                                  const SumRequest* request,
                                  SumResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [Summation::Sum] without a request");
    };

    auto rs = ResourceServer::resource_manager()->resource(request->name());
    if (!rs) {
        return grpc::Status(grpc::UNKNOWN, "resource not found: " + request->name());
    }

    // Downcast resource to Summation.
    const std::shared_ptr<Summation> summation = std::dynamic_pointer_cast<Summation>(rs);

    auto numbers_vec = repeated_double_field_to_vector(request->numbers());
    response->set_sum(summation->sum(numbers_vec));

    return ::grpc::Status();
}

void SummationServer::register_server(std::shared_ptr<Server> server) {
    server->register_service(this);
}

/* Summation client methods */

SummationClient::SummationClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Summation(std::move(name)),
      stub_(SummationService::NewStub(channel)),
      channel_(std::move(channel)){};

double SummationClient::sum(std::vector<double> numbers) {
    SumRequest request;
    SumResponse response;

    grpc::ClientContext ctx;

    *request.mutable_name() = this->name();
    for (double number : numbers) {
        request.add_numbers(number);
    }

    const grpc::Status status = stub_->Sum(&ctx, request, &response);
    if (!status.ok()) {
        throw std::runtime_error(status.error_message());
    }

    return response.sum();
}

/* Summation registration logic. Automatically registers the Summation API upon compiling
 * this file. */

namespace {
bool init() {
    Registry::register_resource(Summation::static_api(), Summation::resource_registration());
    return true;
};

// NOLINTNEXTLINE
const bool summation_inited = init();
}  // namespace
