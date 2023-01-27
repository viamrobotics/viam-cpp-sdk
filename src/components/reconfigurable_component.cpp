#include <components/component_base.hpp>
#include <config/resource.hpp>
#include <registry/registry.hpp>
#include <resource/resource.hpp>
#include <resource/resource_base.hpp>

class ReconfigurableComponent : public ComponentBase {
   public:
    std::function<Name()> name;
    std::function<void(Component, Dependencies)> reconfigure;
};

