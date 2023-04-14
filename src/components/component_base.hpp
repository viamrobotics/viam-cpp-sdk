#pragma once

#include <string>

#include <google/protobuf/struct.pb.h>

#include <common/v1/common.pb.h>

#include <resource/resource_base.hpp>

namespace viam {
namespace cppsdk {

class ComponentBase : public ResourceBase {
   public:
    virtual ResourceType type() override;
    viam::common::v1::ResourceName get_resource_name(std::string name);
    ComponentBase() : ResourceBase("component"){};

   protected:
    explicit ComponentBase(std::string name) : ResourceBase(std::move(name)){};
};

}  // namespace cppsdk
}  // namespace viam
