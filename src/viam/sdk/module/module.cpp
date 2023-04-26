#include <mutex>
#include <viam/sdk/module/module.hpp>

#include <boost/log/trivial.hpp>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <viam/sdk/subtype/subtype.hpp>

namespace viam {
namespace sdk {

Module::Module(std::string addr) : addr_(addr){};

void Module::set_ready() {
    lock_.lock();
    ready_ = true;
    lock_.unlock();
}
std::lock_guard<std::mutex> Module::lock() {
    return std::lock_guard<std::mutex>(lock_);
};
const std::string& Module::name() const {
    return name_;
};
const std::string& Module::addr() const {
    return addr_;
};
const bool Module::ready() const {
    return ready_;
};
HandlerMap_& Module::handles() {
    return handles_;
};
std::shared_ptr<grpc::Channel> Module::channel() {
    return channel_;
};
std::unordered_map<Subtype, std::shared_ptr<SubtypeService>> Module::services() {
    return services_;
};
std::vector<std::shared_ptr<ResourceServerBase>> Module::servers() {
    return servers_;
};

}  // namespace sdk
}  // namespace viam
