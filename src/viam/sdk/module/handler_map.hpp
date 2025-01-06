#pragma once

#include <viam/sdk/common/proto_convert.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace module {
namespace v1 {

class HandlerMap;

}
}  // namespace module
}  // namespace viam

namespace viam {
namespace sdk {

class HandlerMap_ {
   public:
    HandlerMap_() = default;

    void add_model(Model model, const RPCSubtype& subtype);

    const std::unordered_map<RPCSubtype, std::vector<Model>>& handles() const;

   private:
    std::unordered_map<RPCSubtype, std::vector<Model>> handles_;
};

std::ostream& operator<<(std::ostream& os, const HandlerMap_& hm);

namespace proto_convert_details {

template <>
struct to_proto<HandlerMap_> {
    void operator()(const HandlerMap_&, module::v1::HandlerMap*) const;
};

template <>
struct from_proto<module::v1::HandlerMap> {
    HandlerMap_ operator()(const module::v1::HandlerMap*) const;
};

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
