#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <boost/optional/optional.hpp>
#include <grpcpp/impl/service_type.h>

#include <viam/api/component/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_base.hpp>

namespace viam {
namespace sdk {

class ResourceManager {
   public:
    std::shared_ptr<ResourceBase> resource(std::string name);
    void replace_all(std::unordered_map<Name, std::shared_ptr<ResourceBase>> resources);
    void add(Name name, std::shared_ptr<ResourceBase> resource);
    void add(std::string name, std::shared_ptr<ResourceBase> resource);
    void remove(Name name);
    void replace_one(Name name, std::shared_ptr<ResourceBase> resource);
    const std::unordered_map<std::string, std::shared_ptr<ResourceBase>>& resources() const;
    ResourceManager(){};

   private:
    std::mutex lock_;
    std::unordered_map<std::string, std::shared_ptr<ResourceBase>> resources_;
    std::unordered_map<std::string, std::string> short_names_;
    void do_add(Name name, std::shared_ptr<ResourceBase> resource);
    void do_add(std::string name, std::shared_ptr<ResourceBase> resource);
    void do_remove(Name name);
};

}  // namespace sdk
}  // namespace viam
