#include <components/component_base.hpp>
#include <config/resource.hpp>
#include <resource/resource.hpp>
#include <resource/resource_base.hpp>

class ReconfigurableComponent : public ComponentBase {
   public:
    // CR erodkin: virtual functions, or function-as-field?
    // virtual Name name();
    std::function<Name()> name;
    // CR erodkin: should the Reconfigurable argument be a pointer?
    // CR erodkin: make `Dependencies` an actual type so we don't have to define this map everywhere
    std::function<void(Component, std::unordered_map<Name, ResourceBase>)> reconfigure;
    // void reconfigure(Reconfigurable new_resource);
};

