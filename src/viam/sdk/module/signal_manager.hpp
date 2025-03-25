#pragma once

#include <windows.h>
#include <csignal> // Try to get standard signal definitions

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
    /// @throws `std::runtime_error` if the underlying wait was unsuccessful.
    int wait();

    /// @brief Cleanup resources
    ~SignalManager();

   private:
    static BOOL WINAPI ConsoleCtrlHandler(DWORD ctrlType);
    HANDLE eventHandle_;
    static SignalManager* instance_;
    int receivedSignal_;
};

}  // namespace sdk
}  // namespace viam

