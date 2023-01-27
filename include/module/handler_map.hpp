#include <module/v1/module.pb.h>

#include <resource/resource.hpp>

class HandlerMap {
   public:
    std::unordered_map<RPCSubtype, std::vector<Model>> handles;

    viam::module::v1::HandlerMap to_proto();
    static HandlerMap from_proto(viam::module::v1::HandlerMap proto);
};
