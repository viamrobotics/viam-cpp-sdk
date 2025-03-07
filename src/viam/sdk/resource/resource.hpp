#pragma once

#include <unordered_map>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/log/logger.hpp>
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

    /// @brief Returns the `Name` for a particular resource.
    virtual Name get_resource_name() const;

    /// @brief Return the resource's name.
    virtual std::string name() const;

   private:
    std::string name_;

   protected:
    Name get_resource_name(const std::string& type) const;

    LogSource logger_;
};

template <>
struct API::traits<Resource> {
    static API api() {
        return {"rdk", "resource", "Resource"};
    }
};

}  // namespace sdk
}  // namespace viam
