#pragma once

#include <boost/optional/optional.hpp>
#include <resource/resource.hpp>
#include <resource/resource_base.hpp>
#include <services/service_base.hpp>
#include <string>

class SubtypeService : public ServiceBase {
   public:
    std::shared_ptr<ResourceBase> resource(std::string name);
    void replace_all(std::unordered_map<Name, std::shared_ptr<ResourceBase>> resources);
    void add(Name name, std::shared_ptr<ResourceBase> resource);
    void remove(Name name);
    void replace_one(Name name, std::shared_ptr<ResourceBase> resource);
    SubtypeService();

   private:
    std::mutex lock;
    std::unordered_map<std::string, std::shared_ptr<ResourceBase>> resources;
    std::unordered_map<std::string, std::string> short_names;
    void do_add(Name name, std::shared_ptr<ResourceBase> resource);
    void do_remove(Name name);
};
