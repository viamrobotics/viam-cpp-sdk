#pragma once

#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

class Stoppable {
   public:
    virtual ~Stoppable();
    /// @brief Stops a resource from running.
    /// @param extra Extra arguments to pass to the resource's `stop` method.
    virtual void stop(const ProtoStruct& extra) = 0;

    /// @brief Stops a resource from running.
    inline void stop() {
        return stop({});
    }

   protected:
    explicit Stoppable();
};

}  // namespace sdk
}  // namespace viam
