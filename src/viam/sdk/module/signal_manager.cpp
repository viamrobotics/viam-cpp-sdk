#include <viam/sdk/module/signal_manager.hpp>

#include <csignal>
#include <pthread.h>

#include <array>
#include <tuple>

#include <boost/mp11/tuple.hpp>

namespace viam {
namespace sdk {

using signal_map = SignalManager::signal_map;

SignalManager::SignalManager(SignalManager::signal_map&& sig_map)
    : signal_map_(std::move(sig_map)) {
    sigemptyset(&sigset_);

    for_each_entry([this](const auto& handler_entry) {
        if (handler_entry) {
            sigaddset(&sigset_, handler_entry.value);
        }
    });

    pthread_sigmask(SIG_BLOCK, &sigset_, NULL);
}

int SignalManager::wait() {
    int result = 0;
    bool should_continue = false;

    do {
        int sig = 0;
        result = sigwait(&sigset_, &sig);
        for_each_entry([&should_continue, sig](const auto& handler_entry) {
            if (sig == handler_entry.value && handler_entry) {
                should_continue = handler_entry(sig);
            }
        });
    } while (should_continue);

    return result;
}

template <typename Callable>
void SignalManager::for_each_entry(Callable&& c) {
    static constexpr auto pmd_tuple = std::make_tuple(&signal_map::sigint_handler,
                                                      &signal_map::sigterm_handler,
                                                      &signal_map::sigusr1_handler,
                                                      &signal_map::sigusr2_handler);

    namespace mp11 = boost::mp11;
    mp11::tuple_for_each(
        mp11::tuple_transform(
            [this](auto ptr) -> const auto& { return signal_map_.*ptr; },pmd_tuple),
        std::forward<Callable>(c));
}

}  // namespace sdk
}  // namespace viam
