#include <viam/sdk/module/module.hpp>

#include <boost/log/trivial.hpp>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <viam/sdk/resource/resource_manager.hpp>

namespace viam {
namespace sdk {

Module::Module(std::string addr) : addr_(std::move(addr)){};

void Module::set_ready() {
    ready_ = true;
}

const std::string& Module::name() const {
    return name_;
};
const std::string& Module::addr() const {
    return addr_;
};
bool Module::ready() const {
    return ready_;
};
const HandlerMap_& Module::handles() const {
    return handles_;
};
HandlerMap_& Module::mutable_handles() {
    return handles_;
};
const std::shared_ptr<grpc::Channel>& Module::channel() const {
    return channel_;
};

}  // namespace sdk
}  // namespace viam
