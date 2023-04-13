#include <module/module.hpp>

#include <boost/log/trivial.hpp>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <subtype/subtype.hpp>

using namespace viam::cppsdk;

Module::Module(std::string addr) : addr(addr){};

void Module::set_ready() {
    lock.lock();
    ready = true;
    lock.unlock();
}
