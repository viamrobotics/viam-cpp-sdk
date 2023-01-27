#include <resource/resource.hpp>
#include <resource/resource_base.hpp>
#include <services/service_base.hpp>
#include <string>

class SubtypeService : public ServiceBase {
   public:
    ResourceBase resource(std::string name);
    void replace_all(std::unordered_map<Name, ResourceBase> resources);
    void add(Name name, ResourceBase resource);
    void remove(Name name);
    void replace_one(Name name, ResourceBase resource);
    SubtypeService();

   private:
    std::mutex lock;
    std::unordered_map<std::string, ResourceBase> resources;
    std::unordered_map<std::string, std::string> short_names;
};
