#include <app/v1/robot.pb.h>
#include <common/v1/common.pb.h>
#include <grpcpp/support/status.h>
#include <module/v1/module.grpc.pb.h>
#include <module/v1/module.pb.h>
#include <resource/resource.h>
#include <robot/v1/robot.pb.h>
#include <rpc/dial.h>

#include <boost/log/trivial.hpp>

using viam::module::v1::ModuleService;
using Viam::SDK::ViamChannel;
class ModuleClient {
       public:
	std::mutex lock;
	std::unique_ptr<ModuleService::Stub> stub_;
	std::unordered_map<RPCSubtype, std::vector<Model>> handles;

	ModuleClient(ViamChannel channel);
	void add_resource(viam::app::v1::ComponentConfig cfg,
			  std::vector<std::string> dependencies);

	void reconfigure_resource(viam::app::v1::ComponentConfig cfg,
				  std::vector<std::string> dependencies);
	void remove_resource(Name name);

	void ready(std::string address);
};

ModuleClient::ModuleClient(ViamChannel channel) {
	stub_ = std::move(ModuleService::NewStub(channel.channel));
}

// CR erodkin: we should custom define the config type here instead of forcing
// folks to work with the proto-defined version
void ModuleClient::add_resource(viam::app::v1::ComponentConfig cfg,
				std::vector<std::string> dependencies) {
	viam::module::v1::AddResourceRequest req;
	viam::module::v1::AddResourceResponse resp;
	grpc::ClientContext ctx;
	google::protobuf::RepeatedPtrField<
	    viam::app::v1::ResourceLevelServiceConfig> *rs =
	    cfg.mutable_service_configs();

	viam::app::v1::ResourceLevelServiceConfig rp;

	viam::app::v1::ResourceLevelServiceConfig r = rs->at(0);

	for (auto dep : dependencies) {
		*req.mutable_dependencies()->Add() = dep;
	}
	*req.mutable_config() = cfg;

	grpc::Status response = stub_->AddResource(&ctx, req, &resp);
	if (!response.ok()) {
		BOOST_LOG_TRIVIAL(error)
		    << "Error adding resource: " << response.error_message()
		    << response.error_details();
	}
}

void ModuleClient::reconfigure_resource(viam::app::v1::ComponentConfig cfg,
					std::vector<std::string> dependencies) {
	viam::module::v1::ReconfigureResourceRequest req;
	viam::module::v1::ReconfigureResourceResponse resp;
	grpc::ClientContext ctx;

	for (auto dep : dependencies) {
		*req.mutable_dependencies()->Add() = dep;
	}
	*req.mutable_config() = cfg;

	grpc::Status response = stub_->ReconfigureResource(&ctx, req, &resp);
	if (!response.ok()) {
		BOOST_LOG_TRIVIAL(error)
		    << "Error reconfiguring resource: "
		    << response.error_message() << response.error_details();
	}
};

void ModuleClient::remove_resource(Name name) {
	viam::module::v1::RemoveResourceRequest req;
	viam::module::v1::RemoveResourceResponse resp;
	grpc::ClientContext ctx;

	*req.mutable_name() = name.to_string();

	grpc::Status response = stub_->RemoveResource(&ctx, req, &resp);
	if (!response.ok()) {
		BOOST_LOG_TRIVIAL(error)
		    << "Error removing resource: " << response.error_message()
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
		BOOST_LOG_TRIVIAL(error)
		    << "Error getting ready status: "
		    << response.error_message() << response.error_details();
	}

	viam::module::v1::HandlerMap handler_map = resp.handlermap();
	google::protobuf::RepeatedPtrField<viam::module::v1::HandlerDefinition>
	    handlers = handler_map.handlers();
	std::unordered_map<RPCSubtype, std::vector<Model>> new_handles;

	viam::module::v1::HandlerDefinition first = handlers.at(0);
	const google::protobuf::Descriptor *desc = first.descriptor();
	for (auto handler : handlers) {
		std::vector<Model> models;
		viam::robot::v1::ResourceRPCSubtype rpc_subtype =
		    handler.subtype();
		viam::common::v1::ResourceName name = rpc_subtype.subtype();
		std::string namespace_ = name.namespace_();
		std::string resource_type = name.type();
		std::string resource_subtype = name.subtype();
		Subtype subtype(namespace_, resource_type, resource_subtype);
		const google::protobuf::Descriptor *descriptor =
		    handler.GetDescriptor();
		RPCSubtype handle(subtype, *descriptor);
		for (auto mod : handler.models()) {
			try {
				Model model(mod);
				models.push_back(model);
			} catch (std::string error) {
				BOOST_LOG_TRIVIAL(error)
				    << "Error processing model " + mod;
			}
		}
		new_handles.emplace(handle, models);
	}
	lock.lock();
	handles = new_handles;
	lock.unlock();
};

