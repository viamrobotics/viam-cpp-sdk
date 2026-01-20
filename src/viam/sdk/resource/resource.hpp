#pragma once

#include <unordered_map>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/resource/resource_api.hpp>

namespace viam {
namespace sdk {

class Resource;

using Dependencies = std::unordered_map<Name, std::shared_ptr<Resource>>;

namespace log_detail {

struct logger_access;

}  // namespace log_detail

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

    /// @brief Set the log level for log messages originating from this Resource.
    void set_log_level(log_level) const;

   private:
    std::string name_;

   protected:
    friend log_detail::logger_access;

    Name get_resource_name(const std::string& type) const;

    mutable LogSource logger_;
};

template <>
struct API::traits<Resource> {
    static API api() {
        return {"rdk", "resource", "Resource"};
    }
};

namespace log_detail {

// This is an attorney-client helper to avoid making the logger an outright public member of
// Resource.
struct logger_access {
    static LogSource& logger(const Resource&);
};

}  // namespace log_detail

}  // namespace sdk
}  // namespace viam
