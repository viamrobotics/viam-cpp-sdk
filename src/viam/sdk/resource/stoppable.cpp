#include <viam/sdk/resource/stoppable.hpp>

namespace viam {
namespace sdk {

Stoppable::~Stoppable() = default;
Stoppable::Stoppable() = default;

void Stoppable::stop_if_stoppable(const std::shared_ptr<Resource>& resource,
                                  const ProtoStruct& extra) {
    auto stoppable_res = std::dynamic_pointer_cast<Stoppable>(resource);
    if (stoppable_res) {
        stoppable_res->stop(extra);
    }
}

}  // namespace sdk
}  // namespace viam
