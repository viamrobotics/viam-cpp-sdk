#include "api.hpp"

#include <vector>

#include <google/protobuf/descriptor.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/rpc/server.hpp>

#include "../gen/summation.grpc.pb.h"
#include "../gen/summation.pb.h"

using namespace viam::sdk;
using namespace viam::service::summation::v1;

API Summation::api() const {
    return API::get<Summation>();
}

API API::traits<Summation>::api() {
    return {"viam", "service", "summation"};
}

Summation::Summation(std::string name) : Service(std::move(name)){};

/* Summation server methods */

SummationServer::SummationServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)){};

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

    std::vector<double> numbers_vec = {request->numbers().begin(), request->numbers().end()};
    response->set_sum(summation->sum(numbers_vec));

    return ::grpc::Status();
}

/* Summation client methods */

SummationClient::SummationClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Summation(std::move(name)),
      stub_(SummationService::NewStub(channel)),
      channel_(std::move(channel)){};

double SummationClient::sum(std::vector<double> numbers) {
    return make_client_helper(this, *stub_, &StubType::Sum)
        .with([&](auto& request) {
            for (double number : numbers) {
                request.add_numbers(number);
            }
        })
        .invoke([](auto& response) { return response.sum(); });
}
