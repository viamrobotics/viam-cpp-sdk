/// @file components/encoder.hpp
///
/// @brief Defines a `Encoder` component.
#pragma once

#include <string>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>

namespace viam {
namespace sdk {

/// @class Encoder encoder.hpp "components/encoder.hpp"
/// @brief An encoder is a device that is hooked up to motors to report a position
/// @ingroup Encoder
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific encoder implementations. This class cannot be used on its own.
class Encoder : public Component {
   public:
    /// @enum position_type
    enum class position_type : uint8_t {
        // Unspecified position type
        unspecified = 0,
        // Provided by incremental encoders
        ticks_count = 1,
        // Provided by absolute encoders
        angle_degrees = 2
    };

    /// @struct position
    /// @brief reported position.
    struct position {
        float value;
        position_type type;
    };

    /// @struct properties
    /// @brief Encodes the supported modes of this encoder
    struct properties {
        bool ticks_count_supported;
        bool angle_degrees_supported;
    };

    /// @brief Returns position of the encoder which can either be ticks since last zeroing for an
    /// incremental encoder or degrees for an absolute encoder.
    /// @param position_type The type of position you are requesting. If  the driver does not
    /// implement the requested type, this call will return an error. If position type is
    /// `unspecified`, the response will return a default according to the driver.
    inline position get_position(position_type position_type = position_type::unspecified) {
        return get_position({}, position_type);
    }

    /// @brief Returns position of the encoder which can either be ticks since last zeroing for an
    /// incremental encoder or degrees for an absolute encoder.
    /// @param extra Any additional arguments to the method.
    /// @param position_type The type of position you are requesting. If  the driver does not
    /// implement the requested type, this call will return an error. If position type is
    /// `unspecified`, the response will return a default according to the driver.
    virtual position get_position(const ProtoStruct& extra,
                                  position_type position_type = position_type::unspecified) = 0;

    /// @brief Reset the value of the position
    inline void reset_position() {
        return reset_position({});
    }

    /// @brief Reset the value of the position
    /// @param extra Any additional arguments to the method.
    virtual void reset_position(const ProtoStruct& extra) = 0;

    /// @brief Returns a list of all the position_types that are supported by the encoder.
    inline properties get_properties() {
        return get_properties({});
    }

    /// @brief Returns a list of all the position_types that are supported by the encoder.
    /// @param extra Any additional arguments to the method.
    virtual properties get_properties(const ProtoStruct& extra) = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual ProtoStruct do_command(const ProtoStruct& command) = 0;

    /// @brief Returns `GeometryConfig`s associated with the calling encoder.
    /// @return The requested `GeometryConfig`s associated with the component.
    inline std::vector<GeometryConfig> get_geometries() {
        return get_geometries({});
    }

    /// @brief Returns `GeometryConfig`s associated with the calling encoder.
    /// @param extra Any additional arguments to the method.
    /// @return The requested `GeometryConfig`s associated with the component.
    virtual std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) = 0;

    API api() const override;

   protected:
    explicit Encoder(std::string name);
};

template <>
struct API::traits<Encoder> {
    static API api();
};

bool operator==(const Encoder::position& lhs, const Encoder::position& rhs);
bool operator==(const Encoder::properties& lhs, const Encoder::properties& rhs);

}  // namespace sdk
}  // namespace viam
