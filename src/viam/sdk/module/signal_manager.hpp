#pragma once

#include <signal.h>

#include <functional>

namespace viam {
namespace sdk {

/// @class SignalManager
/// @brief Defines handling logic for SIGINT and SIGTERM required by all C++
/// modules, and optionally for handling SIGUSR1 and SIGUSR2.
/// @ingroup Module
class SignalManager {
   public:
    /// @brief A callable object for handling async signals.
    /// Will be called with signal number of the signal which is raised. Return true to indicate
    /// module execution should continue, false if it should stop.
    using SignalHandler = std::function<bool(int)>;

    /// @brief A registry of SignalHandlers for a predefined set of signals.
    /// A default-constructed instance will block on SIGINT and SIGTERM, exiting when they are
    /// raised.
    struct signal_map {
        template <int signal_value>
        struct handler_entry : std::integral_constant<int, signal_value> {
            handler_entry() = default;

            template <typename Callable>
            handler_entry(Callable&& c) : handler(std::move(c)) {}

            operator bool() const {
                return static_cast<bool>(handler);
            }

            bool operator()(int sig) const {
                return handler(sig);
            }

            SignalHandler handler;
        };

        handler_entry<SIGINT> sigint_handler = [](int) { return false; };
        handler_entry<SIGTERM> sigterm_handler = [](int) { return false; };
        handler_entry<SIGUSR1> sigusr1_handler{nullptr};
        handler_entry<SIGUSR2> sigusr2_handler{nullptr};
    };

    /// @brief Creates a new SignalManager which waits until SIGINT or SIGTERM are received.
    explicit SignalManager() : SignalManager(signal_map{}) {}

    /// @brief Creates a SignalManager with blocking set and callback behavior defined by sig_map.
    SignalManager(signal_map&& sig_map);

    /// @brief Waits on and handles the signals specified by signal_map_.
    /// @return The return value of the underlying call to POSIX sigwait.
    int wait();

   private:
    template <typename Callable>
    void for_each_entry(Callable&& c);

    signal_map signal_map_;

    sigset_t sigset_;
};

}  // namespace sdk
}  // namespace viam
