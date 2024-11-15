#include <viam/sdk/module/handler_map.hpp>

namespace viam {
namespace sdk {

const std::unordered_map<RPCSubtype, std::vector<Model>>& HandlerMap_::handles() const {
    return handles_;
}

HandlerMap_::HandlerMap_() {};

void HandlerMap_::add_model(Model model, const RPCSubtype& subtype) {
    handles_[subtype].push_back(std::move(model));
}

std::ostream& operator<<(std::ostream& os, const HandlerMap_& hm) {
    for (const auto& kv : hm.handles_) {
        os << "API: " << kv.first.api().to_string() << '\n';
        for (const Model& model : kv.second) {
            os << "\tModel: " << model.to_string() << '\n';
        }
    }
    return os;
}

}  // namespace sdk
}  // namespace viam
