#define BOOST_TEST_MODULE test module test_urdf_to_dh_params

#include <fstream>
#include <sstream>

#include <boost/test/included/unit_test.hpp>

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/kinematics.hpp>
#include <viam/sdk/referenceframe/private/urdf_to_dh_internals.hpp>
#include <viam/sdk/referenceframe/urdf_to_dh_params.hpp>

using namespace viam::sdk::urdf_to_dh_internals;

namespace {

viam::sdk::KinematicsDataURDF kinematics_from_string(const std::string& xml) {
    std::vector<unsigned char> bytes(xml.begin(), xml.end());
    return viam::sdk::KinematicsDataURDF(std::move(bytes));
}

viam::sdk::KinematicsDataURDF load_urdf(const std::string& filename) {
    std::ifstream f(std::string(VIAMCPPSDK_TEST_TESTFILES_DIR) + "/" + filename, std::ios::binary);
    if (!f) {
        throw std::runtime_error("failed to open " + filename);
    }
    std::ostringstream buf;
    buf << f.rdbuf();
    return kinematics_from_string(buf.str());
}

}  // namespace

namespace viam {
namespace sdktests {

using namespace viam::sdk;

BOOST_AUTO_TEST_CASE(test_pose_in_meters) {
    Eigen::Isometry3d origin = Eigen::Isometry3d::Identity();
    origin.translation() = Eigen::Vector3d(0.5, 0.0, 0.0);
    auto got = pose_in_meters(origin);
    BOOST_CHECK_SMALL(got.translation().x() - 0.5, 1e-12);
    BOOST_CHECK_SMALL(got.translation().y() - 0.0, 1e-12);
    BOOST_CHECK_SMALL(got.translation().z() - 0.0, 1e-12);

    // None -> identity
    auto identity = pose_in_meters(boost::none);
    BOOST_CHECK(identity.isApprox(Eigen::Isometry3d::Identity()));
}

BOOST_AUTO_TEST_CASE(test_common_normal_perpendicular) {
    // Z-axis through origin + Y-axis through (0, 0, 1): perpendicular intersecting lines.
    Eigen::Vector3d z0(0, 0, 1), p0(0, 0, 0);
    Eigen::Vector3d z1(0, 1, 0), p1(0, 0, 1);
    auto r = common_normal(z0, p0, z1, p1);
    BOOST_CHECK(!r.parallel);
    // z0 x z1 = (0,0,1) x (0,1,0) = (-1, 0, 0)
    BOOST_CHECK_SMALL(r.x_dir.x() - (-1.0), 1e-9);
    BOOST_CHECK_SMALL(r.x_dir.y(), 1e-9);
    BOOST_CHECK_SMALL(r.x_dir.z(), 1e-9);
    BOOST_CHECK_SMALL(r.foot0.z() - 1.0, 1e-9);
    BOOST_CHECK_SMALL(r.foot1.z() - 1.0, 1e-9);
}

BOOST_AUTO_TEST_CASE(test_common_normal_parallel) {
    // Two parallel Z-axes separated by 0.5 in X.
    Eigen::Vector3d z0(0, 0, 1), p0(0, 0, 0);
    Eigen::Vector3d z1(0, 0, 1), p1(0.5, 0, 2);
    auto r = common_normal(z0, p0, z1, p1);
    BOOST_CHECK(r.parallel);
    // Perpendicular projected off Z: (0.5, 0, 0) normalized -> (1, 0, 0)
    BOOST_CHECK_SMALL(r.x_dir.x() - 1.0, 1e-9);
    BOOST_CHECK_SMALL(r.x_dir.y(), 1e-9);
    BOOST_CHECK_SMALL(r.x_dir.z(), 1e-9);
}

BOOST_AUTO_TEST_CASE(test_extract_dh_row_ur5e_row1) {
    // Frame 0: origin (0,0,0), Z=(0,0,1), X=(1,0,0).
    // Frame 1: origin (0,0,0.1625), Z=(0,-1,0), X=(1,0,0).
    // Expected: d=0.1625, a=0, alpha=pi/2, theta=0.
    Eigen::Vector3d z_prev(0, 0, 1), x_prev(1, 0, 0), p_prev(0, 0, 0);
    Eigen::Vector3d z_curr(0, -1, 0), x_curr(1, 0, 0), p_curr(0, 0, 0.1625);

    double d, theta, a, alpha;
    extract_dh_row(z_prev, x_prev, p_prev, z_curr, x_curr, p_curr, d, theta, a, alpha);

    BOOST_CHECK_SMALL(d - 0.1625, 1e-9);
    BOOST_CHECK_SMALL(theta, 1e-9);
    BOOST_CHECK_SMALL(a, 1e-9);
    BOOST_CHECK_SMALL(alpha - M_PI / 2.0, 1e-9);
}

BOOST_AUTO_TEST_CASE(test_validate_end_effector_dh_compatible) {
    BOOST_CHECK_NO_THROW(validate_end_effector_dh(Eigen::Vector3d(0, 0, 1),
                                                  Eigen::Vector3d(1, 0, 0),
                                                  Eigen::Vector3d(0, 0, 0),
                                                  Eigen::Vector3d(0, 0, 1)));
}

BOOST_AUTO_TEST_CASE(test_validate_end_effector_dh_x_not_perpendicular) {
    auto substring_match = [](const Exception& e) {
        return std::string(e.what()).find("not perpendicular") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(validate_end_effector_dh(Eigen::Vector3d(0, 0, 1),
                                                   Eigen::Vector3d(1, 0, 0.5),
                                                   Eigen::Vector3d(0, 0, 0),
                                                   Eigen::Vector3d(0, 0, 1)),
                          Exception,
                          substring_match);
}

BOOST_AUTO_TEST_CASE(test_validate_end_effector_dh_origin_out_of_plane) {
    auto substring_match = [](const Exception& e) {
        return std::string(e.what()).find("out of DH plane") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(validate_end_effector_dh(Eigen::Vector3d(0, 0, 1),
                                                   Eigen::Vector3d(1, 0, 0),
                                                   Eigen::Vector3d(0, 0, 0),
                                                   Eigen::Vector3d(0.5, 0.5, 1)),
                          Exception,
                          substring_match);
}

BOOST_AUTO_TEST_CASE(test_walk_urdf_chain_ur5e) {
    auto parsed = parse_urdf(load_urdf("ur5e-real.urdf"));
    auto ordered = walk_urdf_chain(parsed);
    const std::vector<std::string> expected = {
        "base_joint",
        "base_link-base_link_inertia",
        "shoulder_pan_joint",
        "shoulder_lift_joint",
        "elbow_joint",
        "wrist_1_joint",
        "wrist_2_joint",
        "wrist_3_joint",
        "wrist_3_link-ft_frame",
    };
    BOOST_REQUIRE_EQUAL(ordered.size(), expected.size());
    for (std::size_t i = 0; i < expected.size(); ++i) {
        BOOST_CHECK_EQUAL(ordered[i].name, expected[i]);
    }
}

BOOST_AUTO_TEST_CASE(test_walk_urdf_chain_branching) {
    const std::string xml = R"(<?xml version="1.0"?>
<robot name="branch">
  <link name="world"/>
  <link name="a"/>
  <link name="b"/>
  <link name="c"/>
  <joint name="j1" type="fixed">
    <parent link="world"/><child link="a"/>
    <origin rpy="0 0 0" xyz="0 0 0"/>
  </joint>
  <joint name="j2" type="fixed">
    <parent link="a"/><child link="b"/>
    <origin rpy="0 0 0" xyz="0 0 0"/>
  </joint>
  <joint name="j3" type="fixed">
    <parent link="a"/><child link="c"/>
    <origin rpy="0 0 0" xyz="0 0 0"/>
  </joint>
</robot>)";
    auto substring_match = [](const Exception& e) {
        return std::string(e.what()).find("branching") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(
        walk_urdf_chain(parse_urdf(kinematics_from_string(xml))), Exception, substring_match);
}

BOOST_AUTO_TEST_CASE(test_joint_axes_at_rest_ur5e) {
    auto chain = walk_urdf_chain(parse_urdf(load_urdf("ur5e-real.urdf")));
    auto r = joint_axes_at_rest(chain);
    BOOST_REQUIRE_EQUAL(r.axes.size(), 6u);
    BOOST_REQUIRE_EQUAL(r.origins.size(), 6u);

    // Joint 1 (shoulder_pan): axis = world Z, origin = (0, 0, 0.1625).
    BOOST_CHECK_SMALL(r.axes[0].x(), 1e-9);
    BOOST_CHECK_SMALL(r.axes[0].y(), 1e-9);
    BOOST_CHECK_SMALL(r.axes[0].z() - 1.0, 1e-9);
    BOOST_CHECK_SMALL(r.origins[0].x(), 1e-9);
    BOOST_CHECK_SMALL(r.origins[0].y(), 1e-9);
    BOOST_CHECK_SMALL(r.origins[0].z() - 0.1625, 1e-9);

    // Joint 2 (shoulder_lift): axis in world = (0, -1, 0), origin = (0, 0, 0.1625).
    BOOST_CHECK_SMALL(r.origins[1].x(), 1e-9);
    BOOST_CHECK_SMALL(r.origins[1].y(), 1e-9);
    BOOST_CHECK_SMALL(r.origins[1].z() - 0.1625, 1e-9);
    BOOST_CHECK_SMALL(r.axes[1].x(), 1e-9);
    BOOST_CHECK_SMALL(r.axes[1].y() - (-1.0), 1e-9);
    BOOST_CHECK_SMALL(r.axes[1].z(), 1e-9);
}

BOOST_AUTO_TEST_CASE(test_build_dh_frames_ur5e) {
    auto chain = walk_urdf_chain(parse_urdf(load_urdf("ur5e-real.urdf")));
    auto axes_origins = joint_axes_at_rest(chain);
    auto frames = build_dh_frames(axes_origins.axes, axes_origins.origins, axes_origins.end_pose);

    // 7 frames: base + 6 per joint.
    BOOST_REQUIRE_EQUAL(frames.zs.size(), 7u);
    BOOST_REQUIRE_EQUAL(frames.xs.size(), 7u);
    BOOST_REQUIRE_EQUAL(frames.origins.size(), 7u);

    // Frame 0: world.
    BOOST_CHECK_SMALL(frames.zs[0].z() - 1.0, 1e-9);
    BOOST_CHECK_SMALL(frames.xs[0].x() - 1.0, 1e-9);
    BOOST_CHECK_SMALL(frames.origins[0].x(), 1e-9);

    // Frame 1: Z along -Y, X along world X, origin at z=0.1625.
    BOOST_CHECK_SMALL(frames.zs[1].y() - (-1.0), 1e-9);
    BOOST_CHECK_SMALL(frames.xs[1].x() - 1.0, 1e-9);
    BOOST_CHECK_SMALL(frames.origins[1].z() - 0.1625, 1e-9);
}

BOOST_AUTO_TEST_CASE(test_urdf_to_dh_params_ur5e) {
    struct Row {
        const char* name;
        double d, theta, a, alpha;
    };
    const std::vector<Row> expected = {
        {"shoulder_pan_joint", 0.1625, 0, 0, M_PI / 2},
        {"shoulder_lift_joint", 0, 0, -0.425, 0},
        {"elbow_joint", 0, 0, -0.3922, 0},
        {"wrist_1_joint", 0.1333, 0, 0, M_PI / 2},
        {"wrist_2_joint", 0.0997, 0, 0, -M_PI / 2},
        {"wrist_3_joint", 0.0996, 0, 0, 0},
    };
    auto got = urdf_to_dh_params(load_urdf("ur5e-real.urdf"));
    BOOST_REQUIRE_EQUAL(got.size(), expected.size());
    const double tol = 1e-6;
    for (std::size_t i = 0; i < expected.size(); ++i) {
        BOOST_CHECK_EQUAL(got[i].name, expected[i].name);
        BOOST_CHECK_SMALL(got[i].d - expected[i].d, tol);
        BOOST_CHECK_SMALL(got[i].theta - expected[i].theta, tol);
        BOOST_CHECK_SMALL(got[i].a - expected[i].a, tol);
        BOOST_CHECK_SMALL(got[i].alpha - expected[i].alpha, tol);
    }
}

BOOST_AUTO_TEST_CASE(test_parse_urdf_inline) {
    const std::string xml = R"(<?xml version="1.0"?>
<robot name="t">
  <link name="a"/>
  <link name="b"/>
  <joint name="j1" type="fixed">
    <parent link="a"/><child link="b"/>
    <origin rpy="0 0 0" xyz="1 2 3"/>
  </joint>
</robot>)";
    auto joints = parse_urdf(kinematics_from_string(xml));
    BOOST_REQUIRE_EQUAL(joints.size(), 1u);
    BOOST_CHECK_EQUAL(joints[0].name, "j1");
    BOOST_CHECK_EQUAL(joints[0].type, "fixed");
    BOOST_CHECK_EQUAL(joints[0].parent_link, "a");
    BOOST_CHECK_EQUAL(joints[0].child_link, "b");
    BOOST_REQUIRE(joints[0].origin.has_value());
    BOOST_CHECK_SMALL(joints[0].origin->translation().x() - 1.0, 1e-12);
    BOOST_CHECK_SMALL(joints[0].origin->translation().y() - 2.0, 1e-12);
    BOOST_CHECK_SMALL(joints[0].origin->translation().z() - 3.0, 1e-12);
}

BOOST_AUTO_TEST_CASE(test_urdf_to_dh_params_no_revolute_joints) {
    const std::string xml = R"(<?xml version="1.0"?>
<robot name="fixed_only">
  <link name="world"/>
  <link name="a"/>
  <joint name="j1" type="fixed">
    <parent link="world"/><child link="a"/>
    <origin rpy="0 0 0" xyz="0 0 0"/>
  </joint>
</robot>)";
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("no revolute joints") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(urdf_to_dh_params(kinematics_from_string(xml)), Exception, match);
}

BOOST_AUTO_TEST_CASE(test_urdf_to_dh_params_unsupported_prismatic) {
    const std::string xml = R"(<?xml version="1.0"?>
<robot name="has_prismatic">
  <link name="world"/>
  <link name="a"/>
  <joint name="slide" type="prismatic">
    <parent link="world"/><child link="a"/>
    <origin rpy="0 0 0" xyz="0 0 0"/>
    <axis xyz="1 0 0"/>
    <limit effort="0" lower="-1" upper="1" velocity="0"/>
  </joint>
</robot>)";
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("unsupported type") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(urdf_to_dh_params(kinematics_from_string(xml)), Exception, match);
}

BOOST_AUTO_TEST_CASE(test_urdf_to_dh_params_branching) {
    const std::string xml = R"(<?xml version="1.0"?>
<robot name="branching">
  <link name="world"/>
  <link name="a"/>
  <link name="b"/>
  <link name="c"/>
  <joint name="j1" type="revolute">
    <parent link="world"/><child link="a"/>
    <origin rpy="0 0 0" xyz="0 0 0"/>
    <axis xyz="0 0 1"/>
    <limit effort="0" lower="-1" upper="1" velocity="0"/>
  </joint>
  <joint name="j2" type="revolute">
    <parent link="a"/><child link="b"/>
    <origin rpy="0 0 0" xyz="0 0 0"/>
    <axis xyz="0 0 1"/>
    <limit effort="0" lower="-1" upper="1" velocity="0"/>
  </joint>
  <joint name="j3" type="revolute">
    <parent link="a"/><child link="c"/>
    <origin rpy="0 0 0" xyz="0 0 0"/>
    <axis xyz="0 0 1"/>
    <limit effort="0" lower="-1" upper="1" velocity="0"/>
  </joint>
</robot>)";
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("branching") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(urdf_to_dh_params(kinematics_from_string(xml)), Exception, match);
}

namespace {

// Forward-kinematics helper: compose each DH row as
// Rz(theta) * Tz(d) * Tx(a) * Rx(alpha). Matches the Go test helper.
Eigen::Isometry3d dh_forward_kinematics(const std::vector<viam::sdk::DHParam>& params) {
    Eigen::Isometry3d cumulative = Eigen::Isometry3d::Identity();
    for (const auto& p : params) {
        Eigen::Isometry3d step = Eigen::Isometry3d::Identity();
        step.linear() = Eigen::AngleAxisd(p.theta, Eigen::Vector3d::UnitZ()).toRotationMatrix();
        cumulative = cumulative * step;

        step = Eigen::Isometry3d::Identity();
        step.translation() = Eigen::Vector3d(0, 0, p.d);
        cumulative = cumulative * step;

        step = Eigen::Isometry3d::Identity();
        step.translation() = Eigen::Vector3d(p.a, 0, 0);
        cumulative = cumulative * step;

        step = Eigen::Isometry3d::Identity();
        step.linear() = Eigen::AngleAxisd(p.alpha, Eigen::Vector3d::UnitX()).toRotationMatrix();
        cumulative = cumulative * step;
    }
    return cumulative;
}

// End-effector pose at zero configuration, computed by direct URDF composition.
Eigen::Isometry3d urdf_end_pose_at_rest(const viam::sdk::KinematicsDataURDF& urdf) {
    using namespace viam::sdk::urdf_to_dh_internals;
    auto chain = walk_urdf_chain(parse_urdf(urdf));
    Eigen::Isometry3d cumulative = Eigen::Isometry3d::Identity();
    for (const auto& j : chain) {
        cumulative = cumulative * pose_in_meters(j.origin);
    }
    return cumulative;
}

void check_fk_roundtrip(const std::string& fixture) {
    auto urdf = load_urdf(fixture);
    auto params = urdf_to_dh_params(urdf);
    auto dh = dh_forward_kinematics(params);
    auto ref = urdf_end_pose_at_rest(urdf);

    const double tol = 1e-6;
    BOOST_CHECK_SMALL(dh.translation().x() - ref.translation().x(), tol);
    BOOST_CHECK_SMALL(dh.translation().y() - ref.translation().y(), tol);
    BOOST_CHECK_SMALL(dh.translation().z() - ref.translation().z(), tol);

    // Compare rotation matrices element-wise.
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            BOOST_CHECK_SMALL(dh.linear()(r, c) - ref.linear()(r, c), tol);
        }
    }
}

}  // namespace

BOOST_AUTO_TEST_CASE(test_urdf_to_dh_params_ur5e_fk_round_trip) {
    check_fk_roundtrip("ur5e-real.urdf");
}

BOOST_AUTO_TEST_CASE(test_urdf_to_dh_params_gp12_fk_round_trip) {
    auto params = urdf_to_dh_params(load_urdf("gp12.urdf"));
    BOOST_CHECK_EQUAL(params.size(), 6u);
    check_fk_roundtrip("gp12.urdf");
}

BOOST_AUTO_TEST_CASE(test_urdf_to_dh_params_non_dh_compatible_end) {
    const std::string xml = R"(<?xml version="1.0"?>
<robot name="bad_end">
  <link name="world"/>
  <link name="a"/>
  <link name="b"/>
  <joint name="j1" type="revolute">
    <parent link="world"/><child link="a"/>
    <origin rpy="0 0 0" xyz="0 0 0"/>
    <axis xyz="0 0 1"/>
    <limit effort="0" lower="-1" upper="1" velocity="0"/>
  </joint>
  <joint name="tilt" type="fixed">
    <parent link="a"/><child link="b"/>
    <origin rpy="0 0.5 0" xyz="0 0 0"/>
  </joint>
</robot>)";
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("not perpendicular") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(urdf_to_dh_params(kinematics_from_string(xml)), Exception, match);
}

}  // namespace sdktests
}  // namespace viam
