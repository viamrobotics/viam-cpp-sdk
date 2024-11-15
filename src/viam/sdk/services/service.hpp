#pragma once

#include <string>

#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

class Service : public Resource {
   public:
    Name get_resource_name() const override;
    Service();

   protected:
    explicit Service(std::string name) : Resource(std::move(name)){};
};

}  // namespace sdk
}  // namespace viam
