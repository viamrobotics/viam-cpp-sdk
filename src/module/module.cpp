#include <grpcpp/channel.h>
#include <grpcpp/security/credentials.h>

#include "subtype/subtype.hpp"
#define BOOST_LOG_DYN_LINK 1
#include <grpcpp/create_channel.h>

#include <boost/log/trivial.hpp>
#include <module/module.hpp>

Module::Module(std::string addr) : addr(addr) {
    Subtype generic(RDK, COMPONENT, GENERIC);
    std::shared_ptr<SubtypeService> sub_svc = std::make_shared<SubtypeService>();
    this->services.emplace(generic, sub_svc);
};

void Module::set_ready() {
    // // CR erodkin: we can't lock because someone else is owning it but this ain't good. see if we
    // can fix this.
    // lock.lock();
    ready = true;
    // lock.unlock();
}

void Module::dial() {
    // CR erodkin: delete
    std::cout << "WE ARE CALLING DIAL FROM A MODULE!! When does this happen??" << std::endl;
    if (this->channel != nullptr) {
        BOOST_LOG_TRIVIAL(info) << "attempted to dial with module " + this->name +
                                       " but it was already connected.";
        return;
    }

    // // CR erodkin: ??
    // std::string address("unix://");
    // address += this->addr;

    // this->channel = grpc::CreateChannel(this->addr, grpc::InsecureChannelCredentials());
    //  CR erodkin: is this right?
    set_ready();
}

