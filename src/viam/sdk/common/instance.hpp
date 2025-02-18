#pragma once

#include <viam/sdk/registry/registry.hpp>

namespace viam {
namespace sdk {

/// @brief Instance management for Viam C++ SDK applications.
/// This is a single instance class which is responsible for global setup and teardown related to
/// the SDK. An Instance must be constructed before doing anything else in a program, and it must
/// remain alive in a valid state for the duration of the program. Creating multiple overlapping
/// Instance objects is an error.
class Instance {
   public:
    Instance();

    Registry& registry();

   private:
    Registry registry_;
};

}  // namespace sdk
}  // namespace viam
