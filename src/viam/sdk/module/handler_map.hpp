#pragma once

#include <viam/api/module/v1/module.pb.h>

#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

class HandlerMap_ {
   public:
    HandlerMap_();
    void add_model(const Model& model, const RPCSubtype& subtype);

    viam::module::v1::HandlerMap to_proto() const;
    static const HandlerMap_ from_proto(const viam::module::v1::HandlerMap& proto);
    friend std::ostream& operator<<(std::ostream& os, const HandlerMap_& hm);

   private:
    std::unordered_map<RPCSubtype, std::vector<Model>> handles_;
};

}  // namespace sdk
}  // namespace viam
