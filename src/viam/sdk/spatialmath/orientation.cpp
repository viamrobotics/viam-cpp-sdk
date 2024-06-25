#include <viam/sdk/spatialmath/orientation.hpp>

#include <string>
#include <vector>

#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>

#include <viam/api/app/v1/robot.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/spatialmath/orientation_types.hpp>

namespace viam {
namespace sdk {

namespace proto = viam::app::v1;

OrientationConfig::OrientationConfig() {
    type_ = Quaternion;
    quaternion quat;
    quat.w = 1;
    quat.x = 0;
    quat.y = 0;
    quat.z = 0;
    orientation_ = quat;
}

OrientationConfig OrientationConfig::from_proto(const proto::Orientation& proto) {
    OrientationConfig cfg;
    switch (proto.type_case()) {
        case proto::Orientation::TypeCase::kAxisAngles: {
            cfg.type_ = AxisAngles;
            axis_angles aa;
            aa.x = proto.axis_angles().x();
            aa.y = proto.axis_angles().y();
            aa.z = proto.axis_angles().z();
            aa.theta = proto.axis_angles().theta();
            cfg.orientation_ = aa;
            break;
        }
        case proto::Orientation::TypeCase::kEulerAngles: {
            cfg.type_ = EulerAngles;
            euler_angles ea;
            ea.yaw = proto.euler_angles().yaw();
            ea.pitch = proto.euler_angles().pitch();
            ea.roll = proto.euler_angles().roll();
            cfg.orientation_ = ea;
            break;
        }
        case proto::Orientation::TypeCase::kQuaternion: {
            cfg.type_ = Quaternion;
            quaternion quat;
            quat.w = proto.quaternion().w();
            quat.x = proto.quaternion().x();
            quat.y = proto.quaternion().y();
            quat.z = proto.quaternion().z();
            cfg.orientation_ = quat;
            break;
        }
        case proto::Orientation::TypeCase::kVectorDegrees: {
            cfg.type_ = OrientationVectorDegrees;
            orientation_vector_degrees ovd;
            ovd.x = proto.vector_degrees().x();
            ovd.y = proto.vector_degrees().y();
            ovd.z = proto.vector_degrees().z();
            ovd.theta = proto.vector_degrees().theta();
            cfg.orientation_ = ovd;
            break;
        }
        case proto::Orientation::TypeCase::kVectorRadians: {
            cfg.type_ = OrientationVector;
            orientation_vector ov;
            ov.x = proto.vector_radians().x();
            ov.y = proto.vector_radians().y();
            ov.z = proto.vector_radians().z();
            ov.theta = proto.vector_radians().theta();
            cfg.orientation_ = ov;
            break;
        }
        case proto::Orientation::TypeCase::kNoOrientation: {
            // if type is NoOrientation, we put a sentinel
            // orientation that indicates no rotation
            cfg.type_ = Quaternion;
            quaternion quat;
            quat.w = 1;
            quat.x = 0;
            quat.y = 0;
            quat.z = 0;
            cfg.orientation_ = quat;
            break;
        }
        case proto::Orientation::TypeCase::TYPE_NOT_SET:
        default: {
            throw Exception(ErrorCondition::k_not_supported, "orientation type not known");
        }
    }
    return cfg;
}

proto::Orientation OrientationConfig::to_proto() const {
    proto::Orientation orientation;
    switch (type_) {
        case AxisAngles: {
            proto::Orientation_AxisAngles aa;
            const axis_angles a = boost::get<axis_angles>(orientation_);
            aa.set_x(a.x);
            aa.set_y(a.y);
            aa.set_z(a.z);
            aa.set_theta(a.theta);
            *orientation.mutable_axis_angles() = std::move(aa);
            return orientation;
        };
        case OrientationVector: {
            proto::Orientation_OrientationVectorRadians ovec;
            const orientation_vector ov = boost::get<orientation_vector>(orientation_);
            ovec.set_x(ov.x);
            ovec.set_y(ov.y);
            ovec.set_z(ov.z);
            ovec.set_theta(ov.theta);
            *orientation.mutable_vector_radians() = std::move(ovec);
            return orientation;
        };
        case OrientationVectorDegrees: {
            proto::Orientation_OrientationVectorDegrees ovec;
            const orientation_vector_degrees ovd =
                boost::get<orientation_vector_degrees>(orientation_);
            ovec.set_x(ovd.x);
            ovec.set_y(ovd.y);
            ovec.set_z(ovd.z);
            ovec.set_theta(ovd.theta);
            *orientation.mutable_vector_degrees() = std::move(ovec);
            return orientation;
        };
        case EulerAngles: {
            proto::Orientation_EulerAngles euler;
            const euler_angles ea = boost::get<euler_angles>(orientation_);
            euler.set_pitch(ea.pitch);
            euler.set_roll(ea.roll);
            euler.set_yaw(ea.yaw);
            *orientation.mutable_euler_angles() = std::move(euler);
            return orientation;
        };
        case Quaternion: {
            proto::Orientation_Quaternion quat;
            const quaternion q = boost::get<quaternion>(orientation_);
            quat.set_w(q.w);
            quat.set_x(q.x);
            quat.set_y(q.y);
            quat.set_z(q.z);
            *orientation.mutable_quaternion() = std::move(quat);
            return orientation;
        };
        default: {
            throw Exception(ErrorCondition::k_not_supported, "orientation type not known");
        }
    }
}

OrientationType OrientationConfig::get_type() const {
    return type_;
};
const std::vector<std::uint8_t>& OrientationConfig::get_value() const {
    return value_;
};
const orientation& OrientationConfig::get_orientation() const {
    return orientation_;
}

}  // namespace sdk
}  // namespace viam
