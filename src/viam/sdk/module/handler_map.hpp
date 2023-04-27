#pragma once

#include <viam/api/module/v1/module.pb.h>

#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

class HandlerMap_ {
   public:
    HandlerMap_();
    void add_model(Model model, RPCSubtype subtype);

    const viam::module::v1::HandlerMap to_proto() const;
    static HandlerMap_ from_proto(viam::module::v1::HandlerMap proto);

   private:
    std::unordered_map<RPCSubtype, std::vector<Model>> handles_;
};

}  // namespace sdk
}  // namespace viam
