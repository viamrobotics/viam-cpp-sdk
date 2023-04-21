#pragma once

#include <string>

#include <google/protobuf/struct.pb.h>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/resource/resource_base.hpp>

namespace viam {
namespace sdk {

class ComponentBase : public ResourceBase {
   public:
    virtual ResourceType type() override;
    viam::common::v1::ResourceName get_resource_name(std::string name) override;
    ComponentBase() : ResourceBase("component"){};

   protected:
    explicit ComponentBase(std::string name) : ResourceBase(std::move(name)){};
};

}  // namespace sdk
}  // namespace viam
