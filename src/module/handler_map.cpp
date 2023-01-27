#include <module/v1/module.pb.h>

#include <boost/log/trivial.hpp>
#include <resource/resource.hpp>

#include "common/v1/common.pb.h"
#include "robot/v1/robot.pb.h"

class HandlerMap {
    std::unordered_map<RPCSubtype, std::vector<Model>> handles;

    viam::module::v1::HandlerMap to_proto();
    static HandlerMap from_proto(viam::module::v1::HandlerMap proto);
};

viam::module::v1::HandlerMap HandlerMap::to_proto() {
    viam::module::v1::HandlerMap proto;
    for (auto& h : this->handles) {
        viam::module::v1::HandlerDefinition hd;
        for (auto& model : h.second) {
            *hd.mutable_models()->Add() = model.to_string();
        }
        viam::robot::v1::ResourceRPCSubtype rpc_subtype;
        viam::common::v1::ResourceName resource_name = Name(h.first.subtype, "", "").to_proto();
        *rpc_subtype.mutable_subtype() = resource_name;
        *rpc_subtype.mutable_proto_service() = h.first.proto_service_name;
        *hd.mutable_subtype() = rpc_subtype;

        *proto.add_handlers() = hd;
    }

    return proto;
};

HandlerMap HandlerMap::from_proto(viam::module::v1::HandlerMap proto) {
    HandlerMap hm;

    google::protobuf::RepeatedPtrField<viam::module::v1::HandlerDefinition> handlers =
        proto.handlers();

    for (auto handler : handlers) {
        std::vector<Model> models;
        viam::robot::v1::ResourceRPCSubtype rpc_subtype = handler.subtype();
        viam::common::v1::ResourceName name = rpc_subtype.subtype();
        std::string namespace_ = name.namespace_();
        std::string resource_type = name.type();
        std::string resource_subtype = name.subtype();
        Subtype subtype(namespace_, resource_type, resource_subtype);
        const google::protobuf::Descriptor* descriptor = handler.GetDescriptor();
        RPCSubtype handle(subtype, *descriptor);
        for (auto mod : handler.models()) {
            try {
                Model model(mod);
                models.push_back(model);
            } catch (std::string error) {
                BOOST_LOG_TRIVIAL(error) << "Error processing model " + mod;
            }
        }
        hm.handles.emplace(handle, models);
    }

    return hm;
}
