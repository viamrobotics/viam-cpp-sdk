/// @file services/discovery.hpp
///
/// @brief Defines a `Discovery` service.

#pragma once

#include <string>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/services/service.hpp>

namespace viam {
namespace sdk {

/// @defgroup Discovery Classes related to the Discovery service.

/// @class Discovery discovery.hpp "services/discovery.hpp"
/// @brief A `Discovery` service is used to discover resources that are physically connected to
/// your machine.
/// @ingroup Discovery
///
/// This acts as an abstract parent class to be inherited from by any drivers representing
/// specific discovery implementations. This class cannot be used on its own.
class Discovery : public Service {
   public:
    /// @brief Discover valid viam configuration of resources that are physically
    /// connected to your machine.
    /// @return array of potential viam configurations for hardware physically
    /// connected to your viam server
    inline std::vector<ResourceConfig> discover_resources() {
        return discover_resources({});
    }

    /// @brief Discover valid viam configuration of resources that are physically
    /// connected to your machine.
    /// @param extra Any additional arguments to the method.
    /// @return array of potential viam configurations for hardware physically
    // connected to your viam server
    virtual std::vector<ResourceConfig> discover_resources(const ProtoStruct& extra) = 0;

    /// @brief Do an arbitrary command.
    /// @param command Freeform fields that are service-specific.
    /// @return Freeform result of the command.
    virtual ProtoStruct do_command(const ProtoStruct& command) = 0;

    API api() const override;

   protected:
    explicit Discovery(std::string name);
};

template <>
struct API::traits<Discovery> {
    static API api();
};

}  // namespace sdk
}  // namespace viam
