#pragma once

#include <unordered_map>

#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdk {

class Resource;
using Dependencies = std::unordered_map<Name, std::shared_ptr<Resource>>;
class Resource {
   public:
    virtual ~Resource();
    explicit Resource(std::string name);

    /// @brief Returns the `API` associated with a particular resource.
    virtual API api() const = 0;

    /// @brief Returns a `ResourceName` for a particular resource name.
    virtual viam::common::v1::ResourceName get_resource_name(std::string name) const;

    /// @brief Return the resource's name.
    virtual std::string name() const;

    std::vector<Name> resource_names() const;

   private:
    std::string name_;
};

template <>
struct API::traits<Resource> {
    static API api() {
        return {"rdk", "resource", "Resource"};
    }
};

}  // namespace sdk
}  // namespace viam
