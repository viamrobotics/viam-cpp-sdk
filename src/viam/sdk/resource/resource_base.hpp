#pragma once

#include <unordered_map>

#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_type.hpp>

namespace viam {
namespace sdk {

class Resource;
using Dependencies = std::unordered_map<Name, std::shared_ptr<Resource>>;
class Resource {
   public:
    virtual ~Resource();
    explicit Resource(std::string name) : name_(std::move(name)){};
    static Subtype subtype();

    /// @brief Returns the `Subtype` associated with a particular resource.
    virtual Subtype instance_subtype() const = 0;

    /// @brief Returns a `ResourceName` for a particular resource name.
    virtual viam::common::v1::ResourceName get_resource_name(std::string name);

    /// @brief Stops a resource from running.
    /// @param extra Extra arguments to pass to the resource's `stop` method.
    virtual grpc::StatusCode stop(AttributeMap extra);

    /// @brief Stops a resource from running.
    virtual grpc::StatusCode stop();

    /// @brief Reconfigures a resource. By default a no-op.
    /// @param deps Dependencies of the resource.
    /// @param cfg The resource's config.
    virtual void reconfigure(Dependencies deps, ResourceConfig cfg);

    /// @brief Return the resource's name.
    virtual std::string name() const;

    /// @brief Return the resource's type.
    virtual ResourceType type() const;

   private:
    std::string name_;
};

}  // namespace sdk
}  // namespace viam
