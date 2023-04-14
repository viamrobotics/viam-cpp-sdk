#pragma once

#include <string>

#include <common/v1/common.pb.h>

#include <resource/resource_base.hpp>

namespace viam {
namespace cppsdk {

class ServiceBase : public ResourceBase {
   public:
    virtual viam::common::v1::ResourceName get_resource_name(std::string name);
    ResourceType type() override;
    ServiceBase();

   protected:
    explicit ServiceBase(std::string name) : ResourceBase(std::move(name)){};
};

}  // namespace cppsdk
}  // namespace viam
