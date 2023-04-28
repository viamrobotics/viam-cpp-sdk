#pragma once

#include <viam/sdk/resource/resource_manager.hpp>

namespace viam {
namespace sdk {

class ComponentServiceBase {
   protected:
    const ResourceManager& manager() const;

   private:
    ResourceManager manager_;
};

}  // namespace sdk
}  // namespace viam
