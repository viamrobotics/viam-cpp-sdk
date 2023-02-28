#pragma once

#include <grpcpp/impl/service_type.h>

#include <boost/optional/optional.hpp>
#include <string>

#include "component/generic/v1/generic.grpc.pb.h"
#include "resource/resource.hpp"

// TODO (RSDK-1742): create resource_base_fwd.h which does this fwd declaration, and include that
class ResourceBase;

class SubtypeService : public grpc::Service {
   public:
    std::shared_ptr<ResourceBase> resource(std::string name);
    void replace_all(std::unordered_map<Name, std::shared_ptr<ResourceBase>> resources);
    void add(Name name, std::shared_ptr<ResourceBase> resource);
    void remove(Name name);
    void replace_one(Name name, std::shared_ptr<ResourceBase> resource);
    SubtypeService(){};

    static std::shared_ptr<SubtypeService> of_subtype(std::string resource_subtype);
    virtual void register_service();

   private:
    std::mutex lock;
    std::unordered_map<std::string, std::shared_ptr<ResourceBase>> resources;
    std::unordered_map<std::string, std::string> short_names;
    void do_add(Name name, std::shared_ptr<ResourceBase> resource);
    void do_remove(Name name);
};

// TODO(1742): this isn't going to be particularly scalable as we aim to add support for more types,
// figure out a better solution
class GenericSubtypeService : public SubtypeService,
                              public viam::component::generic::v1::GenericService::Service {
   public:
    ::grpc::Status DoCommand(::grpc::ServerContext* context,
                             const ::viam::component::generic::v1::DoCommandRequest* request,
                             ::viam::component::generic::v1::DoCommandResponse* response) override;

    void register_service() override;
    GenericSubtypeService(){};
};
