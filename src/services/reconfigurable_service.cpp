#include <config/resource.hpp>
#include <registry/registry.hpp>
#include <resource/resource.hpp>

class ReconfigurableService : public ServiceBase {
   public:
    std::function<Name()> name;
    std::function<void(Service, Dependencies)> reconfigure;
};
