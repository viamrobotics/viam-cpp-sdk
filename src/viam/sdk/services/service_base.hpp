#pragma once

#include <string>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/resource/resource_base.hpp>

namespace viam {
namespace sdk {

class ServiceBase : public ResourceBase {
   public:
    viam::common::v1::ResourceName get_resource_name(std::string name) override;
    ResourceType type() const override;
    ServiceBase();

   protected:
    explicit ServiceBase(std::string name) : ResourceBase(std::move(name)){};
};

}  // namespace sdk
}  // namespace viam
