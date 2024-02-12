/// @file components/power_sensor/power_sensor.hpp
///
/// @brief Defines a `PowerSensor` component.
#pragma once

#include <string>

#include <viam/api/component/powersensor/v1/powersensor.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>

using namespace viam::component::powersensor::v1;

namespace viam {
namespace sdk {

/// @defgroup PowerSensor Classes related to the PowerSensor component.

/// @class PowerSensor power_sensor.hpp "components/power_sensor/power_sensor.hpp"
/// @ingroup PowerSensor
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific powersensor implementations. This class cannot be used on its own.
class PowerSensor : public Component {
   public:
    /// @struct voltage
    struct voltage {
        /// Voltage in volts.
        double volts;
        /// Whether the voltage is DC or AC.
        bool is_ac;
    };

    /// @struct current
    struct current {
        /// Current in amperes.
        double amperes;
        /// Whether the current is DC or AC.
        bool is_ac;
    };

    API api() const override;

    /// @brief Creates a `voltage` struct from its proto representation.
    static voltage from_proto(const GetVoltageResponse& proto);

    /// @brief Creates a `current` struct from its proto representation.
    static current from_proto(const GetCurrentResponse& proto);

    /// @brief Converts a `voltage` struct to its proto representation.
    static GetVoltageResponse to_proto(voltage v);

    /// @brief Converts a `current` struct to its proto representation.
    static GetCurrentResponse to_proto(current c);

    /// @brief Returns the voltage reading of this sensor.
    /// @return The voltage reading of this sensor.
    inline voltage get_voltage() {
        return get_voltage({});
    }

    /// @brief Returns the voltage reading of this sensor.
    /// @param extra Any additional arguments to this method.
    /// @return The voltage reading of this sensor.
    virtual voltage get_voltage(const AttributeMap& extra) = 0;

    /// @brief Returns the current reading of this sensor.
    /// @return The current reading of this sensor.
    inline current get_current() {
        return get_current({});
    }

    /// @brief Returns the current reading of this sensor.
    /// @param extra Any additional arguments to this method.
    /// @return The current reading of this sensor.
    virtual current get_current(const AttributeMap& extra) = 0;

    /// @brief Returns the power reading of this sensor.
    /// @return The power reading of this sensor.
    inline double get_power() {
        return get_power({});
    }

    /// @brief Returns the power reading of this sensor.
    /// @param extra Any additional arguments to this method.
    /// @return The power reading of this sensor.
    virtual double get_power(const AttributeMap& extra) = 0;

    /// @brief Returns the measurements/data specific to this sensor.
    /// @return The requested measurements/data specific to this sensor.
    inline AttributeMap get_readings() {
        return get_readings({});
    }

    /// @brief Returns the measurements/data specific to this sensor.
    /// @param extra Any additional arguments to the method.
    /// @return The requested measurements/data specific to this sensor.
    virtual AttributeMap get_readings(const AttributeMap& extra) = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(const AttributeMap& command) = 0;

   protected:
    explicit PowerSensor(std::string name) : Component(std::move(name)){};
};

template <>
struct API::traits<PowerSensor> {
    static API api();
};

bool operator==(const PowerSensor::voltage& lhs, const PowerSensor::voltage& rhs);
bool operator==(const PowerSensor::current& lhs, const PowerSensor::current& rhs);
}  // namespace sdk
}  // namespace viam
