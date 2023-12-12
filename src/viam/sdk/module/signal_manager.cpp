#include <viam/sdk/module/signal_manager.hpp>

#include <csignal>
#include <pthread.h>

namespace viam {
namespace sdk {

SignalManager::SignalManager() {
    sigemptyset(&sigset_);
    sigaddset(&sigset_, SIGINT);
    sigaddset(&sigset_, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &sigset_, NULL);
}

int SignalManager::wait() {
    int sig = 0;
    return sigwait(&sigset_, &sig);
}

}  // namespace sdk
}  // namespace viam
