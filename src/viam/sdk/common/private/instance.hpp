#pragma once

#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/registry/registry.hpp>

namespace viam {
namespace sdk {

struct Instance::Impl {
    Registry registry;
    LogManager log_mgr;

    // When dialing directly to gpc (no rust-utils/webrtc) there is global state in the form of a
    // bearer token that needs to be added as metadata to every client call. This variable stores
    // the auth token; see client_helper.cpp and dial.cpp for access.
    std::string direct_dial_token;
};

}  // namespace sdk
}  // namespace viam
