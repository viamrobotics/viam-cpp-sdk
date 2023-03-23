#include <grpcpp/channel.h>
#include <grpcpp/security/credentials.h>

#include "subtype/subtype.hpp"
#define BOOST_LOG_DYN_LINK 1
#include <grpcpp/create_channel.h>

#include <boost/log/trivial.hpp>
#include <module/module.hpp>

Module::Module(std::string addr) : addr(addr){};

void Module::set_ready() {
    lock.lock();
    ready = true;
    lock.unlock();
}
