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
    /// @brief Enumeration for creation behavior of @ref current
    enum class Creation {
        open_existing,  ///< Instance must already exist
        if_needed       ///< Use existing instance if present, else create one.
    };

    Instance();
    ~Instance();

    /// @brief Get the current Instance according to the Creation behavior.
    /// Calling current(Creation::open_existing) when an instance has not yet been constructed is an
    /// error.
    static Instance& current(Creation);

   private:
    friend class Registry;

    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace sdk
}  // namespace viam
