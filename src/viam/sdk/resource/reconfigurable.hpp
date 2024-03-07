#pragma once

#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

class Reconfigurable {
   public:
    virtual ~Reconfigurable();

    /// @brief Reconfigures a resource.
    /// @param deps Dependencies of the resource.
    /// @param cfg The resource's config.
    virtual void reconfigure(const Dependencies& deps, const ResourceConfig& cfg) = 0;

    /// @brief Reconfigures a resource if it is Reconfigurable.
    /// @param resource the Resource to reconfigure.
    /// @param deps Dependencies of the resource.
    /// @param cfg The resource's config.
    static void reconfigure_if_reconfigurable(const std::shared_ptr<Resource>& resource,
                                              const Dependencies& deps,
                                              const ResourceConfig& cfg);

   protected:
    explicit Reconfigurable();
};

}  // namespace sdk
}  // namespace viam
