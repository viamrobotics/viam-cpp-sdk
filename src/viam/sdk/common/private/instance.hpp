#pragma once

#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/registry/registry.hpp>

namespace viam {
namespace sdk {

struct Instance::Impl {
    Registry registry;
    LogManager log_mgr;
};

}  // namespace sdk
}  // namespace viam
