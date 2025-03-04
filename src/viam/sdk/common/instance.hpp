#pragma once

#include <memory>

namespace viam {
namespace sdk {

/// @brief Instance management for Viam C++ SDK applications.
/// This is a single instance class which is responsible for global setup and teardown related to
/// the SDK. An Instance must be constructed before doing anything else in a program, and it must
/// remain alive in a valid state for the duration of the program. Creating multiple Instance
/// objects in the same program is an error.
class Instance {
   public:
    Instance();
    ~Instance();

    static Instance& current();

   private:
    friend class Registry;
    friend class Logger;

    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace sdk
}  // namespace viam
