#include <viam/sdk/module/module.hpp>

#include <boost/log/sinks.hpp>
#include <boost/log/trivial.hpp>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <viam/sdk/resource/resource_manager.hpp>

namespace viam {
namespace sdk {

namespace logging = boost::log;

Module::Module(std::string addr) : addr_(std::move(addr)) {};

void Module::set_ready() {
    ready_ = true;
}

// namespace impl {
// class my_buf_ : public std::stringbuf {
// public:
// virtual int sync() {
// auto msg = this->str();
// parent_->return 0;
//}

// private:
// std::unique_ptr<Module> parent_;
//};
//}  // namespace impl

// void Module::init_logging(const std::shared_ptr<RobotClient>& parent) {
// typedef logging::sinks::synchronous_sink<logging::sinks::text_ostream_backend> text_sink;
// auto sink = boost::make_shared<text_sink>();
// sink->locked_backend()->add_stream();
// logging::core::get()->add_sink(sink);
//}

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
