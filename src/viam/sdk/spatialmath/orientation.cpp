#include <viam/sdk/spatialmath/orientation.hpp>

#include <string>
#include <vector>

#include <boost/variant/apply_visitor.hpp>

#include <viam/api/app/v1/robot.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/spatialmath/orientation_types.hpp>

namespace viam {
namespace sdk {

OrientationType get_type(const Orientation& o) {
    struct Visitor {
        auto operator()(const quaternion&) const {
            return OrientationType::Quaternion;
        }

        auto operator()(const axis_angles&) const {
            return OrientationType::AxisAngles;
        }

        auto operator()(const orientation_vector&) const {
            return OrientationType::OrientationVector;
        }

        auto operator()(const orientation_vector_degrees&) const {
            return OrientationType::OrientationVectorDegrees;
        }

        auto operator()(const euler_angles&) const {
            return OrientationType::EulerAngles;
        }
    };

    return boost::apply_visitor(Visitor{}, o);
}

namespace proto_convert_details {

void to_proto<Orientation>::operator()(const Orientation& self, app::v1::Orientation* o) const {
    struct Visitor {
        void operator()(const axis_angles& a) {
            app::v1::Orientation_AxisAngles aa;
            aa.set_x(a.x);
            aa.set_y(a.y);
            aa.set_z(a.z);
            aa.set_theta(a.theta);
            *orientation.mutable_axis_angles() = std::move(aa);
        }

        void operator()(const orientation_vector& ov) {
            app::v1::Orientation_OrientationVectorRadians ovec;
            ovec.set_x(ov.x);
            ovec.set_y(ov.y);
            ovec.set_z(ov.z);
            ovec.set_theta(ov.theta);
            *orientation.mutable_vector_radians() = std::move(ovec);
        }

        void operator()(const orientation_vector_degrees& ovd) {
            app::v1::Orientation_OrientationVectorDegrees ovec;
            ovec.set_x(ovd.x);
            ovec.set_y(ovd.y);
            ovec.set_z(ovd.z);
            ovec.set_theta(ovd.theta);
            *orientation.mutable_vector_degrees() = std::move(ovec);
        }

        void operator()(const euler_angles& ea) {
            app::v1::Orientation_EulerAngles euler;
            euler.set_pitch(ea.pitch);
            euler.set_roll(ea.roll);
            euler.set_yaw(ea.yaw);
            *orientation.mutable_euler_angles() = std::move(euler);
        }

        void operator()(const quaternion& q) {
            app::v1::Orientation_Quaternion quat;
            quat.set_w(q.w);
            quat.set_x(q.x);
            quat.set_y(q.y);
            quat.set_z(q.z);
            *orientation.mutable_quaternion() = std::move(quat);
        }

        app::v1::Orientation& orientation;
    };

    boost::apply_visitor(Visitor{*o}, self);
}

Orientation from_proto<app::v1::Orientation>::operator()(const app::v1::Orientation* proto) const {
    switch (proto->type_case()) {
        case app::v1::Orientation::TypeCase::kAxisAngles: {
            axis_angles aa;
            aa.x = proto->axis_angles().x();
            aa.y = proto->axis_angles().y();
            aa.z = proto->axis_angles().z();
            aa.theta = proto->axis_angles().theta();
            return aa;
        }
        case app::v1::Orientation::TypeCase::kEulerAngles: {
            euler_angles ea;
            ea.yaw = proto->euler_angles().yaw();
            ea.pitch = proto->euler_angles().pitch();
            ea.roll = proto->euler_angles().roll();
            return ea;
        }
        case app::v1::Orientation::TypeCase::kQuaternion: {
            quaternion quat;
            quat.w = proto->quaternion().w();
            quat.x = proto->quaternion().x();
            quat.y = proto->quaternion().y();
            quat.z = proto->quaternion().z();
            return quat;
        }
        case app::v1::Orientation::TypeCase::kVectorDegrees: {
            orientation_vector_degrees ovd;
            ovd.x = proto->vector_degrees().x();
            ovd.y = proto->vector_degrees().y();
            ovd.z = proto->vector_degrees().z();
            ovd.theta = proto->vector_degrees().theta();
            return ovd;
        }
        case app::v1::Orientation::TypeCase::kVectorRadians: {
            orientation_vector ov;
            ov.x = proto->vector_radians().x();
            ov.y = proto->vector_radians().y();
            ov.z = proto->vector_radians().z();
            ov.theta = proto->vector_radians().theta();
            return ov;
        }
        case app::v1::Orientation::TypeCase::kNoOrientation: {
            // if type is NoOrientation, we put a sentinel
            // orientation that indicates no rotation
            quaternion quat;
            quat.w = 1;
            quat.x = 0;
            quat.y = 0;
            quat.z = 0;
            return quat;
        }
        case app::v1::Orientation::TypeCase::TYPE_NOT_SET:
        default: {
            throw Exception(ErrorCondition::k_not_supported, "orientation type not known");
        }
    }
}

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam
