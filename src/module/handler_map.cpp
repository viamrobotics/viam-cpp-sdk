#include <module/handler_map.hpp>

#include <memory>

#include <boost/log/trivial.hpp>
#include <google/protobuf/descriptor.h>

#include <common/v1/common.pb.h>
#include <module/v1/module.pb.h>
#include <robot/v1/robot.pb.h>

#include <resource/resource.hpp>

namespace viam {
namespace cppsdk {

viam::module::v1::HandlerMap HandlerMap_::to_proto() {
    viam::module::v1::HandlerMap proto;
    for (auto& h : this->handles) {
        viam::module::v1::HandlerDefinition hd;
        for (auto& model : h.second) {
            const std::string m = model.to_string();
            *hd.mutable_models()->Add() = m;
        }
        viam::robot::v1::ResourceRPCSubtype rpc_subtype;
        Name name(h.first.subtype, "", "");
        viam::common::v1::ResourceName resource_name = name.to_proto();
        *rpc_subtype.mutable_subtype() = resource_name;
        *rpc_subtype.mutable_proto_service() = h.first.proto_service_name;
        *hd.mutable_subtype() = rpc_subtype;
        *proto.add_handlers() = hd;
    }

    return proto;
};

HandlerMap_::HandlerMap_(){};

HandlerMap_ HandlerMap_::from_proto(viam::module::v1::HandlerMap proto) {
    HandlerMap_ hm;

    google::protobuf::RepeatedPtrField<viam::module::v1::HandlerDefinition> handlers =
        proto.handlers();

    for (auto& handler : handlers) {
        std::vector<Model> models;
        viam::common::v1::ResourceName name = handler.subtype().subtype();
        Subtype subtype(name.namespace_(), name.type(), name.subtype());
        const google::protobuf::DescriptorPool* pool =
            google::protobuf::DescriptorPool::generated_pool();
        const google::protobuf::ServiceDescriptor* sd = pool->FindServiceByName(name.type());
        RPCSubtype handle(subtype, *sd);
        for (auto& mod : handler.models()) {
            try {
                Model model = Model::from_str(mod);
                models.push_back(model);
            } catch (std::string error) {
                BOOST_LOG_TRIVIAL(error) << "Error processing model " + mod;
            }
        }
        hm.handles.emplace(handle, models);
    }

    return hm;
}

void HandlerMap_::add_model(Model model, RPCSubtype subtype) {
    std::vector<Model> models;
    if (handles.find(subtype) != handles.end()) {
        models = handles.at(subtype);
    }
    models.push_back(model);
    handles.emplace(subtype, models);
}

}  // namespace cppsdk
}  // namespace viam
