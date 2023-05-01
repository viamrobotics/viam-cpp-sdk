/// @file components/encoder/encoder.hpp
///
/// @brief Defines a `Encoder` component.
#pragma once

#include <string>

#include <viam/api/component/encoder/v1/encoder.pb.h>

#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource_manager.hpp>

namespace viam {
namespace sdk {

/// @defgroup Encoder Classes related to the `Encoder` component.

/// @class EncoderSubtype
/// @brief Defines a `ResourceSubtype` for the `Encoder` component.
/// @ingroup Encoder
class EncoderSubtype : public ResourceSubtype {
   public:
    std::shared_ptr<ResourceServerBase> create_resource_server(
        std::shared_ptr<ResourceManager> manager) override;
    std::shared_ptr<ResourceBase> create_rpc_client(std::string name,
                                                    std::shared_ptr<grpc::Channel> chan) override;
    EncoderSubtype(const google::protobuf::ServiceDescriptor* service_descriptor)
        : ResourceSubtype(service_descriptor){};
};

/// @class Encoder encoder.hpp "components/encoder/encoder.hpp"
/// @brief An encoder is a device that is hooked up to motors to report a position
/// @ingroup Encoder
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific encoder implementations. This class cannot be used on its own.
class Encoder : public ComponentBase {
   public:
    /// @enum position_type
    enum position_type {
        // Unspecified position type
        UNSPECIFIED,
        // Provided by incremental encoders
        TICKS_COUNT,
        // Provided by absolute encoders
        ANGLE_DEGREES
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

    // functions shared across all components
    static std::shared_ptr<ResourceSubtype> resource_subtype();
    static Subtype subtype();

    /// @brief Creates a `properties` struct from its proto representation.
    static position_type from_proto(viam::component::encoder::v1::PositionType proto);

    /// @brief Creates a `position` struct from its proto representation.
    static position from_proto(viam::component::encoder::v1::GetPositionResponse proto);

    /// @brief Creates a `properties` struct from its proto representation.
    static properties from_proto(viam::component::encoder::v1::GetPropertiesResponse proto);

    /// @brief Converts a `position` struct to its proto representation.
    static viam::component::encoder::v1::PositionType to_proto(position_type position_type);

    /// @brief Converts a `position` struct to its proto representation.
    static viam::component::encoder::v1::GetPositionResponse to_proto(position position);

    /// @brief Converts a `properties` struct to its proto representation.
    static viam::component::encoder::v1::GetPropertiesResponse to_proto(properties properties);

    /// @brief Returns position of the encoder which can either be ticks since last zeroing for an
    /// incremental encoder or degrees for an absolute encoder.
    /// @param position_type If supplied, the response will return the specified position type. If
    /// the driver does not implement the requested type, this call will return an error. If
    /// position type is UNSPECIFIED, the response will return a default according to the driver.
    virtual position get_position(position_type position_type) = 0;

    /// @brief Reset the value of the position
    virtual void reset_position() = 0;

    /// @brief Returns a list of all the position_types that are supported by the encoder.
    virtual properties get_properties() = 0;

    /// @brief Send/receive arbitrary commands to the resource.
    /// @param Command the command to execute.
    /// @return The result of the executed command.
    virtual AttributeMap do_command(AttributeMap command) = 0;

   protected:
    explicit Encoder(std::string name) : ComponentBase(std::move(name)){};
};

bool operator==(const Encoder::position& lhs, const Encoder::position& rhs);
bool operator==(const Encoder::properties& lhs, const Encoder::properties& rhs);

}  // namespace sdk
}  // namespace viam
