#pragma once

#include <memory>  //must include for std::unique_ptr

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
    struct Impl;                  // Forward declaration for pImpl idiom
    std::unique_ptr<Impl> pImpl;  // Pointer to the implementation
};

}  // namespace sdk
}  // namespace viam
