#pragma once

#include <string>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/resource/resource_base.hpp>

namespace viam {
namespace sdk {

class Service : public Resource {
   public:
    viam::common::v1::ResourceName get_resource_name(std::string name) override;
    ResourceType type() override;
    Service();

   protected:
    explicit Service(std::string name) : Resource(std::move(name)){};
};

}  // namespace sdk
}  // namespace viam
