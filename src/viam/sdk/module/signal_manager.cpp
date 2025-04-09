#include <viam/sdk/module/signal_manager.hpp>

#ifdef _WIN32
#include <windows.h>
#else
#include <csignal>
#include <pthread.h>
#endif

#include <stdexcept>

namespace viam {
namespace sdk {

#ifdef _WIN32
SignalManager* SignalManager::instance_ = nullptr;
#endif

SignalManager::SignalManager() {
#ifdef _WIN32
    SignalManager::instance_ = this;
    SetConsoleCtrlHandler(SignalManager::console_handler_routine_, TRUE);
#else
    sigemptyset(&sigset_);
    sigaddset(&sigset_, SIGINT);
    sigaddset(&sigset_, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &sigset_, NULL);
#endif
}

SignalManager::~SignalManager() {
#ifdef _WIN32
    SetConsoleCtrlHandler(NULL, FALSE);
    if (signal_event_ != INVALID_HANDLE_VALUE) {
        CloseHandle(signal_event_);
    }
    SignalManager::instance_ = nullptr;
#endif
}

int SignalManager::wait() {
#ifdef _WIN32
    signal_event_ = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (signal_event_ == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Failed to create signal event");
    }
    WaitForSingleObject(signal_event_, INFINITE);
    return static_cast<DWORD>(signal_code_);
#else
    int sig = 0;
    return sigwait(&sigset_, &sig);
#endif
}

#ifdef _WIN32

BOOL WINAPI SignalManager::console_handler_routine_(DWORD dwCtrlType) {
    if (dwCtrlType == CTRL_C_EVENT || dwCtrlType == CTRL_BREAK_EVENT) {
        SignalManager::instance_->handle_signal_(SIGINT);
        return TRUE;
    } else if (dwCtrlType == CTRL_CLOSE_EVENT || dwCtrlType == CTRL_LOGOFF_EVENT ||
               dwCtrlType == CTRL_SHUTDOWN_EVENT) {
        SignalManager::instance_->handle_signal_(SIGTERM);
        return TRUE;
    }
    return FALSE;
}

void SignalManager::handle_signal_(int signal) {
    if (signal_event_ != INVALID_HANDLE_VALUE) {
        signal_code_ = signal;
        SetEvent(signal_event_);
    }
}

#endif

}  // namespace sdk
}  // namespace viam
