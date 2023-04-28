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
#include <viam/sdk/subtype/subtype.hpp>

namespace viam {
namespace sdk {

/// @defgroup Encoder Classes related to the `Encoder` component.

/// @class EncoderSubtype
/// @brief Defines a `ResourceSubtype` for the `Encoder` component.
/// @ingroup Encoder
class EncoderSubtype : public ResourceSubtype {
   public:
    std::shared_ptr<ResourceServerBase> create_resource_server(
        std::shared_ptr<SubtypeService> svc) override;
    std::shared_ptr<ResourceBase> create_rpc_client(std::string name,
                                                    std::shared_ptr<grpc::Channel> chan) override;
    EncoderSubtype(const google::protobuf::ServiceDescriptor* service_descriptor)
        : ResourceSubtype(service_descriptor){};
};

/// @class Encoder encoder.hpp "components/encoder/encoder.hpp"
/// @brief TODO
/// @ingroup Encoder
///
/// This acts as an abstract base class to be inherited from by any drivers representing
/// specific encoder implementations. This class cannot be used on its own.
class Encoder : public ComponentBase {
   public:
    /// @enum position_type
    /// @brief TODO.
    enum position_type { UNSPECIFIED, TICKS_COUNT, ANGLE_DEGREES };

    /// @struct position
    /// @brief TODO.
    struct position {
        /// TODO
        float value;
        /// TODO
        position_type type;
    };

    /// @struct properties
    /// @brief TODO.
    struct properties {
        /// TODO
        bool ticks_count_supported;
        /// TODO
        bool angle_degrees_supported;
    };

    // functions shared across all components
    static std::shared_ptr<ResourceSubtype> resource_subtype();
    static Subtype subtype();

    /// @brief Creates a `position` struct from its proto representation.
    static position from_proto(viam::component::encoder::v1::GetPositionResponse proto);

    /// @brief Creates a `properties` struct from its proto representation.
    static properties from_proto(viam::component::encoder::v1::GetPropertiesResponse proto);

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

    /// @brief TODO
    virtual void reset_position() = 0;

    /// @brief Returns a list of all the position_types that are supported by the encoder.
    virtual properties get_properties() = 0;

    /// @brief TODO
    /// @param TODO TODO
    virtual AttributeMap do_command(AttributeMap command) = 0;

   protected:
    explicit Encoder(std::string name) : ComponentBase(std::move(name)){};
};

bool operator==(const Encoder::position& lhs, const Encoder::position& rhs);
bool operator==(const Encoder::properties& lhs, const Encoder::properties& rhs);

}  // namespace sdk
}  // namespace viam
