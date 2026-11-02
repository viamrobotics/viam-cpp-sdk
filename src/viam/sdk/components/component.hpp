#pragma once

#include <string>

#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

class Component : public Resource {
   public:
    Component();
    Name get_resource_name() const override;

   protected:
    explicit Component(std::string name);

    explicit Component(const ResourceConfig& cfg);
};

}  // namespace sdk
}  // namespace viam
