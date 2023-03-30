#pragma once

#include <viam/api/module/v1/module.pb.h>

#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

class HandlerMap_ {
   public:
    HandlerMap_();
    void add_model(Model model, RPCSubtype subtype);
    std::unordered_map<RPCSubtype, std::vector<Model>> handles;

    viam::module::v1::HandlerMap to_proto();
    static HandlerMap_ from_proto(viam::module::v1::HandlerMap proto);
};

}  // namespace sdk
}  // namespace viam
