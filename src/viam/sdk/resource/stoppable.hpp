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

    /// @brief Stops a Resource if it is Stoppable.
    /// @param resource The Resource to stop.
    /// @param extra Extra arguments to pass to the resource's `stop` method.
    static void stop_if_stoppable(const std::shared_ptr<Resource>& resource,
                                  const ProtoStruct& extra);

    /// @brief Stops a Resource if it is Stoppable.
    /// @param resource The Resource to stop.
    inline static void stop_if_stoppable(const std::shared_ptr<Resource>& resource) {
        return stop_if_stoppable(resource, {});
    }

   protected:
    explicit Stoppable();
};

}  // namespace sdk
}  // namespace viam
