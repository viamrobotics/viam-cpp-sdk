#include <viam/sdk/spatialmath/orientation.hpp>

#include <string>
#include <vector>

#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>

#include <viam/api/app/v1/robot.pb.h>

#include <viam/sdk/spatialmath/orientation_types.hpp>

namespace viam {
namespace sdk {

namespace proto = viam::app::v1;

OrientationConfig OrientationConfig::from_proto(proto::Orientation proto) {
    OrientationConfig cfg;
    switch (proto.type_case()) {
        case proto::Orientation::TypeCase::kAxisAngles: {
            cfg.type_ = AxisAngles;
            axis_angles aa;
            aa.x_ = proto.axis_angles().x();
            aa.y_ = proto.axis_angles().y();
            aa.z_ = proto.axis_angles().z();
            aa.theta_ = proto.axis_angles().theta();
            cfg.orientation_ = aa;
            break;
        }
        case proto::Orientation::TypeCase::kEulerAngles: {
            cfg.type_ = EulerAngles;
            euler_angles ea;
            ea.yaw_ = proto.euler_angles().yaw();
            ea.pitch_ = proto.euler_angles().pitch();
            ea.roll_ = proto.euler_angles().roll();
            cfg.orientation_ = ea;
            break;
        }
        case proto::Orientation::TypeCase::kQuaternion: {
            cfg.type_ = Quaternion;
            quaternion quat;
            quat.w_ = proto.quaternion().w();
            quat.x_ = proto.quaternion().x();
            quat.y_ = proto.quaternion().y();
            quat.z_ = proto.quaternion().z();
            cfg.orientation_ = quat;
            break;
        }
        case proto::Orientation::TypeCase::kVectorDegrees: {
            cfg.type_ = OrientationVectorDegrees;
            orientation_vector_degrees ovd;
            ovd.x_ = proto.vector_degrees().x();
            ovd.y_ = proto.vector_degrees().y();
            ovd.z_ = proto.vector_degrees().z();
            ovd.theta_ = proto.vector_degrees().theta();
            cfg.orientation_ = ovd;
            break;
        }
        case proto::Orientation::TypeCase::kVectorRadians: {
            cfg.type_ = OrientationVector;
            orientation_vector ov;
            ov.x_ = proto.vector_radians().x();
            ov.y_ = proto.vector_radians().y();
            ov.z_ = proto.vector_radians().z();
            ov.theta_ = proto.vector_radians().theta();
            cfg.orientation_ = ov;
            break;
        }
        case proto::Orientation::TypeCase::kNoOrientation: {
            // if type is NoOrientation, we put a sentinel
            // orientation that indicates no rotation
            cfg.type_ = Quaternion;
            quaternion quat;
            quat.w_ = 1;
            quat.x_ = 0;
            quat.y_ = 0;
            quat.z_ = 0;
            cfg.orientation_ = quat;
            break;
        }
        case proto::Orientation::TypeCase::TYPE_NOT_SET:
        default: {
            throw std::runtime_error("orientation type not known");
        }
    }
    return cfg;
}

proto::Orientation OrientationConfig::to_proto() {
    proto::Orientation orientation;
    switch (type_) {
        case AxisAngles: {
            proto::Orientation_AxisAngles aa;
            axis_angles a = boost::get<axis_angles>(orientation_);
            aa.set_x(a.x_);
            aa.set_y(a.y_);
            aa.set_z(a.z_);
            aa.set_theta(a.theta_);
            orientation.set_allocated_axis_angles(&aa);
            return orientation;
        };
        case OrientationVector: {
            proto::Orientation_OrientationVectorRadians ovec;
            orientation_vector ov = boost::get<orientation_vector>(orientation_);
            ovec.set_x(ov.x_);
            ovec.set_y(ov.y_);
            ovec.set_z(ov.z_);
            ovec.set_theta(ov.theta_);
            orientation.set_allocated_vector_radians(&ovec);
            return orientation;
        };
        case OrientationVectorDegrees: {
            proto::Orientation_OrientationVectorDegrees ovec;
            orientation_vector_degrees ovd = boost::get<orientation_vector_degrees>(orientation_);
            ovec.set_x(ovd.x_);
            ovec.set_y(ovd.y_);
            ovec.set_z(ovd.z_);
            ovec.set_theta(ovd.theta_);
            orientation.set_allocated_vector_degrees(&ovec);
            return orientation;
        };
        case EulerAngles: {
            proto::Orientation_EulerAngles euler;
            euler_angles ea = boost::get<euler_angles>(orientation_);
            euler.set_pitch(ea.pitch_);
            euler.set_roll(ea.roll_);
            euler.set_yaw(ea.yaw_);
            orientation.set_allocated_euler_angles(&euler);
            return orientation;
        };
        case Quaternion: {
            proto::Orientation_Quaternion quat;
            quaternion q = boost::get<quaternion>(orientation_);
            quat.set_w(q.w_);
            quat.set_x(q.x_);
            quat.set_y(q.y_);
            quat.set_z(q.z_);
            orientation.set_allocated_quaternion(&quat);
            return orientation;
        };
        default: {
            throw std::runtime_error("orientation type not known");
        }
    }
}

}  // namespace sdk
}  // namespace viam
