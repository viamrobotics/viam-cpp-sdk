#pragma once

#include <string>

#include <google/protobuf/struct.pb.h>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/resource/resource_base.hpp>

namespace viam {
namespace sdk {

class Component : public Resource {
   public:
    virtual ResourceType type() const override;
    viam::common::v1::ResourceName get_resource_name(std::string name) override;
    Component() : Resource("component"){};

   protected:
    explicit Component(std::string name) : Resource(std::move(name)){};
};

}  // namespace sdk
}  // namespace viam
