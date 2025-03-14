#pragma once

#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/log/logger.hpp>
#include <viam/sdk/registry/registry.hpp>

namespace viam {
namespace sdk {

struct Instance::Impl {
    Registry registry;
    Logger logger;
};

}  // namespace sdk
}  // namespace viam
