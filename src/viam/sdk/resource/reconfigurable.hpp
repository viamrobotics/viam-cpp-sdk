#pragma once

#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

class Reconfigurable {
   public:
    virtual ~Reconfigurable();

   protected:
    explicit Reconfigurable();
};

}  // namespace sdk
}  // namespace viam
