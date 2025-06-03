#include <viam/sdk/module/signal_manager.hpp>

#include <csignal>
#include <pthread.h>
#include <signal.h>

namespace viam {
namespace sdk {

struct SignalManager::Impl {
    sigset_t sigset_;
};

SignalManager::SignalManager() : pImpl(std::make_unique<Impl>()) {
    sigemptyset(&(pImpl->sigset_));
    sigaddset(&(pImpl->sigset_), SIGINT);
    sigaddset(&(pImpl->sigset_), SIGTERM);
    pthread_sigmask(SIG_BLOCK, &(pImpl->sigset_), NULL);
}

SignalManager::~SignalManager() {}

int SignalManager::wait() {
    int sig = 0;
    return sigwait(&((pImpl->sigset_)), &sig);
}

}  // namespace sdk
}  // namespace viam