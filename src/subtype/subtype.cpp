#include <resource/resource.hpp>
#include <resource/resource_base.hpp>
#include <services/service_base.hpp>
#include <string>
#include <subtype/subtype.hpp>

// CR erodkin: define all of these. should key in replace_all be a name?

ResourceBase resource(std::string name){};
void SubtypeService::replace_all(std::unordered_map<std::string, ResourceBase> resources){};
void SubtypeService::add(Name name, ResourceBase resource){};
void SubtypeService::remove(Name name){};
void SubtypeService::replace_one(Name name, ResourceBase resource){};

