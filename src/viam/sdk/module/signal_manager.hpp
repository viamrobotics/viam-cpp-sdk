#pragma once

#include <signal.h>

#ifdef _WIN32
#include <windows.h>
#endif

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
#ifdef _WIN32
    static BOOL WINAPI console_handler_routine_(DWORD dwCtrlType);
    void handle_signal_(int signal);

    static SignalManager* instance_;
    HANDLE signal_event_ = INVALID_HANDLE_VALUE;
    int signal_code_ = 0;
#else
    sigset_t sigset_;
#endif
};

}  // namespace sdk
}  // namespace viam
