#pragma once

#include <string>

#include <google/protobuf/struct.pb.h>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

class Component : public Resource {
   public:
    Component();
    viam::common::v1::ResourceName get_resource_name(std::string name) const override;

   protected:
    explicit Component(std::string name);
};

}  // namespace sdk
}  // namespace viam
