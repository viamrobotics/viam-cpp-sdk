#pragma once

#include <signal.h>

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

    /// @brief Wait for SignalManager to receive SIGINT or SIGTERM.
    /// @return The signal number if successful.
    /// @throws runtime_error if the underlying sigwait call was unsuccessful.
    int wait();

   private:
    sigset_t sigset_;
};

}  // namespace sdk
}  // namespace viam
