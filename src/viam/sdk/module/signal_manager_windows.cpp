#include <signal.h>
#include <viam/sdk/module/signal_manager.hpp>
#include <windows.h>  //this file assumes WIN32

#include <stdexcept>

namespace viam {
namespace sdk {

struct SignalManager::Impl {  // Windows implementation details of SignalManager
    static BOOL WINAPI console_handler_routine_(DWORD dwCtrlType) {
        if (dwCtrlType == CTRL_C_EVENT || dwCtrlType == CTRL_BREAK_EVENT) {
            instance_->pImpl->handle_signal_(SIGINT);
            return TRUE;
        } else if (dwCtrlType == CTRL_CLOSE_EVENT || dwCtrlType == CTRL_LOGOFF_EVENT ||
                   dwCtrlType == CTRL_SHUTDOWN_EVENT) {
            instance_->pImpl->handle_signal_(SIGTERM);
            return TRUE;
        }
        return FALSE;
    }

    void handle_signal_(int signal) {
        if (signal_event_ != INVALID_HANDLE_VALUE) {
            signal_code_ = signal;
            SetEvent(signal_event_);
        }
    }

    static SignalManager* instance_;
    HANDLE signal_event_ = INVALID_HANDLE_VALUE;
    int signal_code_ = 0;
};

SignalManager* SignalManager::Impl::instance_ = nullptr;

SignalManager::SignalManager() : pImpl(std::make_unique<Impl>()) {
    SignalManager::Impl::instance_ = this;
    SetConsoleCtrlHandler(SignalManager::Impl::console_handler_routine_, TRUE);
}

SignalManager::~SignalManager() {
    SetConsoleCtrlHandler(NULL, FALSE);
    if (pImpl->signal_event_ != INVALID_HANDLE_VALUE) {
        CloseHandle(pImpl->signal_event_);
    }
    SignalManager::Impl::instance_ = nullptr;
}

int SignalManager::wait() {
    pImpl->signal_event_ = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (pImpl->signal_event_ == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Failed to create signal event");
    }
    WaitForSingleObject(pImpl->signal_event_, INFINITE);
    return static_cast<DWORD>(pImpl->signal_code_);
}

}  // namespace sdk
}  // namespace viam
