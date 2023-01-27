#include <app/v1/robot.pb.h>
#include <common/v1/common.pb.h>
#include <grpcpp/support/status.h>
#include <module/v1/module.grpc.pb.h>
#include <module/v1/module.pb.h>
#include <robot/v1/robot.pb.h>

#include <boost/log/trivial.hpp>
#include <config/resource.hpp>
#include <module/handler_map.hpp>
#include <resource/resource.hpp>
#include <rpc/dial.hpp>

using viam::module::v1::ModuleService;
using Viam::SDK::ViamChannel;
class ModuleClient {
   public:
    std::mutex lock;
    std::unique_ptr<ModuleService::Stub> stub_;
    HandlerMap handles;

    ModuleClient(ViamChannel channel);
    void add_resource(Component cfg, std::vector<std::string> dependencies);

    void reconfigure_resource(Component cfg, std::vector<std::string> dependencies);
    void remove_resource(Name name);

    void ready(std::string address);
};

ModuleClient::ModuleClient(ViamChannel channel) {
    stub_ = std::move(ModuleService::NewStub(channel.channel));
}

void ModuleClient::add_resource(Component cfg, std::vector<std::string> dependencies) {
    viam::module::v1::AddResourceRequest req;
    viam::module::v1::AddResourceResponse resp;
    grpc::ClientContext ctx;

    viam::app::v1::ComponentConfig proto_cfg = cfg.to_proto();

    for (auto dep : dependencies) {
        *req.mutable_dependencies()->Add() = dep;
    }
    *req.mutable_config() = proto_cfg;

    grpc::Status response = stub_->AddResource(&ctx, req, &resp);
    if (!response.ok()) {
        BOOST_LOG_TRIVIAL(error) << "Error adding resource: " << response.error_message()
                                 << response.error_details();
    }
}

void ModuleClient::reconfigure_resource(Component cfg, std::vector<std::string> dependencies) {
    viam::module::v1::ReconfigureResourceRequest req;
    viam::module::v1::ReconfigureResourceResponse resp;
    grpc::ClientContext ctx;

    for (auto dep : dependencies) {
        *req.mutable_dependencies()->Add() = dep;
    }
    *req.mutable_config() = cfg.to_proto();

    grpc::Status response = stub_->ReconfigureResource(&ctx, req, &resp);
    if (!response.ok()) {
        BOOST_LOG_TRIVIAL(error) << "Error reconfiguring resource: " << response.error_message()
                                 << response.error_details();
    }
};

void ModuleClient::remove_resource(Name name) {
    viam::module::v1::RemoveResourceRequest req;
    viam::module::v1::RemoveResourceResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_name() = name.to_string();

    grpc::Status response = stub_->RemoveResource(&ctx, req, &resp);
    if (!response.ok()) {
        BOOST_LOG_TRIVIAL(error) << "Error removing resource: " << response.error_message()
                                 << response.error_details();
    }
};

void ModuleClient::ready(std::string address) {
    viam::module::v1::ReadyRequest req;
    viam::module::v1::ReadyResponse resp;
    grpc::ClientContext ctx;

    *req.mutable_parent_address() = address;

    grpc::Status response = stub_->Ready(&ctx, req, &resp);
    if (!response.ok()) {
        BOOST_LOG_TRIVIAL(error) << "Error getting ready status: " << response.error_message()
                                 << response.error_details();
    }

    viam::module::v1::HandlerMap proto = resp.handlermap();
    HandlerMap handler_map = HandlerMap::from_proto(proto);

    lock.lock();
    handles = handler_map;
    lock.unlock();
};
