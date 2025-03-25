#include <viam/sdk/module/signal_manager.hpp>

#include <stdexcept>

namespace viam {
namespace sdk {

// Initialize static member
SignalManager* SignalManager::instance_ = nullptr;

// Windows console control handler
BOOL WINAPI SignalManager::ConsoleCtrlHandler(DWORD ctrlType) {
    int signalValue = 0;
    
    switch (ctrlType) {
        case CTRL_C_EVENT:        // Equivalent to SIGINT
            signalValue = SIGINT;
            break;
        case CTRL_BREAK_EVENT:    // Similar to SIGTERM
        case CTRL_CLOSE_EVENT:    // Window closing
        case CTRL_LOGOFF_EVENT:   // User logging off
        case CTRL_SHUTDOWN_EVENT: // System shutting down
            signalValue = SIGTERM;
            break;
        default:
            return FALSE; // Let other handlers process this
    }
    
    if (instance_) {
        instance_->receivedSignal_ = signalValue;
        SetEvent(instance_->eventHandle_);
        return TRUE; // We handled it
    }
    
    return FALSE;
}

SignalManager::SignalManager() : receivedSignal_(0) {
    // Create an event that will be signaled when we receive a console signal
    eventHandle_ = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (eventHandle_ == NULL) {
        throw std::runtime_error("Failed to create Windows event for signal handling");
    }
    
    // Store the instance pointer for the static handler
    instance_ = this;
    
    // Register Windows-specific console handler
    if (!SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE)) {
        CloseHandle(eventHandle_);
        throw std::runtime_error("Failed to set Windows console control handler");
    }
}

SignalManager::~SignalManager() {
    // Unregister the console handler
    SetConsoleCtrlHandler(ConsoleCtrlHandler, FALSE);
    
    // Close the event handle
    if (eventHandle_ != NULL) {
        CloseHandle(eventHandle_);
        eventHandle_ = NULL;
    }
    
    instance_ = nullptr;
}

int SignalManager::wait() {
    // Wait for the event to be signaled (infinite timeout)
    DWORD result = WaitForSingleObject(eventHandle_, INFINITE);
    
    if (result == WAIT_OBJECT_0) {
        return receivedSignal_;
    } else {
        throw std::runtime_error("Wait for signal failed");
    }
}

}  // namespace sdk
}  // namespace viam
