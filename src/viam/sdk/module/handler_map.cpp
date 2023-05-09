#include <viam/sdk/module/handler_map.hpp>

#include <memory>

#include <boost/log/trivial.hpp>
#include <google/protobuf/descriptor.h>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/module/v1/module.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

viam::module::v1::HandlerMap HandlerMap_::to_proto() const {
    viam::module::v1::HandlerMap proto;
    for (const auto& h : this->handles_) {
        viam::module::v1::HandlerDefinition hd;
        for (const auto& model : h.second) {
            const std::string m = model.to_string();
            *hd.mutable_models()->Add() = m;
        }
        viam::robot::v1::ResourceRPCSubtype rpc_subtype;
        Name name(h.first.api(), "", "");
        viam::common::v1::ResourceName resource_name = name.to_proto();
        *rpc_subtype.mutable_subtype() = resource_name;
        *rpc_subtype.mutable_proto_service() = h.first.proto_service_name();
        *hd.mutable_subtype() = rpc_subtype;
        *proto.add_handlers() = hd;
    }

    return proto;
};

HandlerMap_::HandlerMap_(){};

// NOLINTNEXTLINE(readability-const-return-type)
const HandlerMap_ HandlerMap_::from_proto(viam::module::v1::HandlerMap proto) {
    HandlerMap_ hm;

    google::protobuf::RepeatedPtrField<viam::module::v1::HandlerDefinition> handlers =
        proto.handlers();

    for (const auto& handler : handlers) {
        std::vector<Model> models;
        viam::common::v1::ResourceName name = handler.subtype().subtype();
        API api(name.namespace_(), name.type(), name.subtype());
        const google::protobuf::DescriptorPool* pool =
            google::protobuf::DescriptorPool::generated_pool();
        const google::protobuf::ServiceDescriptor* sd = pool->FindServiceByName(name.type());
        RPCSubtype handle(api, *sd);
        for (const auto& mod : handler.models()) {
            try {
                Model model = Model::from_str(mod);
                models.push_back(model);
            } catch (std::string error) {  // NOLINT
                BOOST_LOG_TRIVIAL(error) << "Error processing model " + mod;
            }
        }
        hm.handles_.emplace(handle, models);
    }

    return hm;
}

void HandlerMap_::add_model(Model model, RPCSubtype subtype) {
    std::vector<Model> models;
    if (handles_.find(subtype) != handles_.end()) {
        models = handles_.at(subtype);
    }
    models.push_back(model);
    handles_.emplace(subtype, models);
}

}  // namespace sdk
}  // namespace viam
