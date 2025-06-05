#pragma once

#include <memory>

namespace viam {
namespace sdk {

/// @class SignalManager
/// @brief Defines handling logic for SIGINT and SIGTERM required by all C++
/// modules.
/// @ingroup Module
class SignalManager {
   public:
    /// @brief Creates a new SignalManager.
    explicit SignalManager();

    /// @brief Destructor.
    ~SignalManager();

    /// @brief Wait for SignalManager to receive SIGINT or SIGTERM.
    /// @return The signal number if successful.
    /// @throws `std::runtime_error` if the underlying wait operation was unsuccessful.
    int wait();

   private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};

}  // namespace sdk
}  // namespace viam
