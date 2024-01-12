/// @file components/sensor/sensor.hpp
///
/// @brief Defines a `Sensor` component.
#pragma once

#include <string>

#include <viam/api/component/sensor/v1/sensor.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/sensor/server.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>

namespace viam {
namespace sdk {

/// @defgroup Sensor Classes related to the Sensor component.

/// @class Sensor sensor.hpp "components/sensor/sensor.hpp"
/// @ingroup Sensor
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific sensor implementations. This class cannot be used on its own.
class Sensor : public Component {
   public:
    API api() const override;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(const AttributeMap& command) = 0;

    /// @brief Returns `GeometryConfig`s associated with the calling sensor.
    /// @return The requested `GeometryConfig`s associated with the component.
    inline std::vector<GeometryConfig> get_geometries() {
        return get_geometries({});
    }

    /// @brief Returns `GeometryConfig`s associated with the calling sensor.
    /// @param extra Any additional arguments to the method.
    /// @return The requested `GeometryConfig`s associated with the component.
    virtual std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) = 0;

    /// @brief Returns the measurements/data specific to this sensor.
    /// @return The requested measurements/data specific to this sensor.
    inline AttributeMap get_readings() {
        return get_readings({});
    }

    /// @brief Returns the measurements/data specific to this sensor.
    /// @param extra Any additional arguments to the method.
    /// @return The requested measurements/data specific to this sensor.
    virtual AttributeMap get_readings(const AttributeMap& extra) = 0;

   protected:
    explicit Sensor(std::string name) : Component(std::move(name)){};
};

template <>
struct API::api_map<Sensor> {
    static API api() {
        return {kRDK, kComponent, "sensor"};
    }
};

}  // namespace sdk
}  // namespace viam
