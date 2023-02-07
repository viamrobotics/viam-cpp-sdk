#include <grpcpp/channel.h>
#include <grpcpp/security/credentials.h>

#include "subtype/subtype.hpp"
#define BOOST_LOG_DYN_LINK 1
#include <grpcpp/create_channel.h>

#include <boost/log/trivial.hpp>
#include <module/module.hpp>

Module::Module(std::string addr) : addr(addr) {
    Subtype generic(RDK, COMPONENT, GENERIC);
    SubtypeService sub_svc;
    this->services.emplace(generic, sub_svc);
};

void Module::set_ready() {
    BOOST_LOG_TRIVIAL(error) << "setting ready";
    std::cout << "setting ready";
    // lock.lock();
    ready = true;
    // lock.unlock();

    BOOST_LOG_TRIVIAL(error) << "we set ready in module";
}

void Module::dial() {
    if (this->channel != nullptr) {
        BOOST_LOG_TRIVIAL(info) << "attempted to dial with module " + this->name +
                                       " but it was already connected.";
        return;
    }
    // CR erodkin: is this right?
    set_ready();
    std::cout << "dialin!\n";

    // CR erodkin: double "unix://" in here, figure out how to avoid that
    std::string address("unix://");
    address += this->addr;
    // std::string address = addr;

    this->channel = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
}

