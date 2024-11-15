#pragma once

#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdk {

class HandlerMap_ {
   public:
    HandlerMap_();
    void add_model(Model model, const RPCSubtype& subtype);

    friend std::ostream& operator<<(std::ostream& os, const HandlerMap_& hm);
    const std::unordered_map<RPCSubtype, std::vector<Model>>& handles() const;

   private:
    std::unordered_map<RPCSubtype, std::vector<Model>> handles_;
};

}  // namespace sdk
}  // namespace viam
