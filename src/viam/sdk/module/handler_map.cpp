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

void HandlerMap_::add_model(Model model, const RPCSubtype& subtype) {
    handles_[subtype].push_back(std::move(model));
}

const std::unordered_map<RPCSubtype, std::vector<Model>>& HandlerMap_::handles() const {
    return handles_;
}

std::ostream& operator<<(std::ostream& os, const HandlerMap_& hm) {
    for (const auto& kv : hm.handles()) {
        os << "API: " << kv.first.api().to_string() << '\n';
        for (const Model& model : kv.second) {
            os << "\tModel: " << model.to_string() << '\n';
        }
    }
    return os;
}

namespace proto_convert_details {

void to_proto<HandlerMap_>::operator()(const HandlerMap_& self,
                                       module::v1::HandlerMap* proto) const {
    for (const auto& h : self.handles()) {
        viam::module::v1::HandlerDefinition hd;
        for (const auto& model : h.second) {
            *hd.mutable_models()->Add() = model.to_string();
        }

        viam::robot::v1::ResourceRPCSubtype rpc_subtype;

        *rpc_subtype.mutable_subtype() = v2::to_proto(Name(h.first.api(), "", ""));
        *rpc_subtype.mutable_proto_service() = h.first.proto_service_name();
        *hd.mutable_subtype() = rpc_subtype;

        *proto->add_handlers() = hd;
    }
}

HandlerMap_ from_proto<module::v1::HandlerMap>::operator()(
    const module::v1::HandlerMap* proto) const {
    HandlerMap_ hm;

    const google::protobuf::RepeatedPtrField<viam::module::v1::HandlerDefinition>& handlers =
        proto->handlers();

    for (const auto& handler : handlers) {
        const viam::common::v1::ResourceName name = handler.subtype().subtype();
        const API api(name.namespace_(), name.type(), name.subtype());
        const google::protobuf::DescriptorPool* pool =
            google::protobuf::DescriptorPool::generated_pool();
        const google::protobuf::ServiceDescriptor* sd = pool->FindServiceByName(name.type());
        const RPCSubtype handle(api, *sd);
        for (const auto& mod : handler.models()) {
            try {
                hm.add_model(Model::from_str(mod), handle);
            } catch (const std::exception& ex) {  // NOLINT
                BOOST_LOG_TRIVIAL(error) << "Error " << ex.what() << " processing model " + mod;
            }
        }
    }

    return hm;
}

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam
