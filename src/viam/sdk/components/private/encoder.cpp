#include <viam/sdk/components/private/encoder.hpp>

#include <viam/sdk/common/exception.hpp>

namespace viam {
namespace sdk {
namespace impl {

Encoder::position_type from_proto(viam::component::encoder::v1::PositionType proto) {
    switch (proto) {
        case viam::component::encoder::v1::POSITION_TYPE_UNSPECIFIED: {
            return Encoder::position_type::unspecified;
        }
        case viam::component::encoder::v1::POSITION_TYPE_ANGLE_DEGREES: {
            return Encoder::position_type::angle_degrees;
        }
        case viam::component::encoder::v1::POSITION_TYPE_TICKS_COUNT: {
            return Encoder::position_type::ticks_count;
        }
        default: {
            throw Exception(ErrorCondition::k_not_supported,
                            "Invalid proto encoder type to decode");
        }
    }
}

viam::component::encoder::v1::PositionType to_proto(Encoder::position_type position_type) {
    switch (position_type) {
        case Encoder::position_type::unspecified: {
            return viam::component::encoder::v1::PositionType::POSITION_TYPE_UNSPECIFIED;
        }
        case Encoder::position_type::angle_degrees: {
            return viam::component::encoder::v1::POSITION_TYPE_ANGLE_DEGREES;
        }
        case Encoder::position_type::ticks_count: {
            return viam::component::encoder::v1::POSITION_TYPE_TICKS_COUNT;
        }
        default: {
            throw Exception(ErrorCondition::k_not_supported,
                            "Invalid proto encoder type to encode");
        }
    }
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
