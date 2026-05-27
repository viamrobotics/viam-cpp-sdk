#define BOOST_TEST_MODULE test module test_kinematics_model_table

#include <fstream>
#include <sstream>
#include <string>

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/referenceframe/kinematics_model_table.hpp>
#include <viam/sdk/referenceframe/private/kinematics_model_table_internals.hpp>

using namespace viam::sdk;
using namespace viam::sdk::urdf_model_table_internals;

namespace {
KinematicsDataURDF urdf_from_string(const std::string& xml) {
    std::vector<unsigned char> bytes(xml.begin(), xml.end());
    return KinematicsDataURDF(std::move(bytes));
}
}  // namespace

BOOST_AUTO_TEST_CASE(parse_urdf_minimal) {
    const std::string xml = R"(<?xml version="1.0"?>
<robot name="r">
  <link name="base"/>
  <link name="l1"/>
  <joint name="j1" type="revolute">
    <parent link="base"/>
    <child link="l1"/>
    <origin xyz="1 2 3" rpy="0.1 0.2 0.3"/>
    <axis xyz="0 0 1"/>
  </joint>
</robot>)";
    auto parsed = parse_urdf(urdf_from_string(xml));
    BOOST_REQUIRE_EQUAL(parsed.size(), 1u);
    BOOST_CHECK_EQUAL(parsed[0].name, "j1");
    BOOST_CHECK_EQUAL(parsed[0].type_str, "revolute");
    BOOST_CHECK_EQUAL(parsed[0].parent_link, "base");
    BOOST_CHECK_EQUAL(parsed[0].child_link, "l1");
    BOOST_CHECK_CLOSE(parsed[0].xyz.x(), 1.0, 1e-9);
    BOOST_CHECK_CLOSE(parsed[0].xyz.z(), 3.0, 1e-9);
    BOOST_CHECK_CLOSE(parsed[0].rpy.y(), 0.2, 1e-9);
    BOOST_REQUIRE(parsed[0].axis_opt.has_value());
    BOOST_CHECK_CLOSE(parsed[0].axis_opt->z(), 1.0, 1e-9);
}

BOOST_AUTO_TEST_CASE(parse_urdf_missing_origin) {
    const std::string xml = R"(<?xml version="1.0"?>
<robot name="r">
  <link name="a"/><link name="b"/>
  <joint name="j" type="fixed"><parent link="a"/><child link="b"/></joint>
</robot>)";
    auto parsed = parse_urdf(urdf_from_string(xml));
    BOOST_REQUIRE_EQUAL(parsed.size(), 1u);
    BOOST_CHECK(parsed[0].xyz == Vector3{});
    BOOST_CHECK(parsed[0].rpy == Vector3{});
}

BOOST_AUTO_TEST_CASE(parse_urdf_missing_axis) {
    const std::string xml = R"(<?xml version="1.0"?>
<robot name="r">
  <link name="a"/><link name="b"/>
  <joint name="j" type="revolute"><parent link="a"/><child link="b"/></joint>
</robot>)";
    auto parsed = parse_urdf(urdf_from_string(xml));
    BOOST_REQUIRE_EQUAL(parsed.size(), 1u);
    BOOST_CHECK(!parsed[0].axis_opt.has_value());
}

BOOST_AUTO_TEST_CASE(parse_urdf_malformed_xml) {
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("failed to parse URDF XML") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(parse_urdf(urdf_from_string("<not<valid>>")), Exception, match);
}

BOOST_AUTO_TEST_CASE(parse_urdf_no_robot_root) {
    const std::string xml = R"(<?xml version="1.0"?><notrobot/>)";
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("no <robot> root element") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(parse_urdf(urdf_from_string(xml)), Exception, match);
}

BOOST_AUTO_TEST_CASE(parse_urdf_missing_required_attribute_throws) {
    const std::string xml = R"(<?xml version="1.0"?>
<robot name="r">
  <joint name="j" type="revolute"/>
</robot>)";
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("missing required attribute") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(parse_urdf(urdf_from_string(xml)), Exception, match);
}

namespace {
ParsedJoint mk(const std::string& name,
               const std::string& parent,
               const std::string& child,
               const std::string& type = "revolute") {
    ParsedJoint j;
    j.name = name;
    j.type_str = type;
    j.parent_link = parent;
    j.child_link = child;
    return j;
}
}  // namespace

BOOST_AUTO_TEST_CASE(walk_urdf_chain_serial_ok) {
    std::vector<ParsedJoint> in{mk("j2", "l1", "l2"), mk("j1", "base", "l1"), mk("j3", "l2", "l3")};
    auto out = walk_urdf_chain(in);
    BOOST_REQUIRE_EQUAL(out.size(), 3u);
    BOOST_CHECK_EQUAL(out[0].name, "j1");
    BOOST_CHECK_EQUAL(out[1].name, "j2");
    BOOST_CHECK_EQUAL(out[2].name, "j3");
}

BOOST_AUTO_TEST_CASE(walk_urdf_chain_branching_throws) {
    std::vector<ParsedJoint> in{
        mk("j1", "base", "l1"), mk("j2a", "l1", "l2"), mk("j2b", "l1", "l3")};
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("branching topology") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(walk_urdf_chain(in), Exception, match);
}

BOOST_AUTO_TEST_CASE(walk_urdf_chain_multiple_roots_throws) {
    std::vector<ParsedJoint> in{mk("j1", "baseA", "l1"), mk("j2", "baseB", "l2")};
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("expected exactly one root link") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(walk_urdf_chain(in), Exception, match);
}

BOOST_AUTO_TEST_CASE(walk_urdf_chain_disconnected_throws) {
    std::vector<ParsedJoint> in{
        mk("j1", "base", "l1"), mk("j2", "l1", "l2"), mk("j3", "floatA", "floatB")};
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("expected exactly one root link") != std::string::npos ||
               std::string(e.what()).find("chain walk visited") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(walk_urdf_chain(in), Exception, match);
}

BOOST_AUTO_TEST_CASE(walk_urdf_chain_cycle_throws) {
    // base -> A -> B -> A  (back-edge from B to A creates a cycle reachable from
    // the valid root `base`).
    std::vector<ParsedJoint> in{mk("j1", "base", "A"), mk("j2", "A", "B"), mk("j3", "B", "A")};
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("cycle in URDF chain") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(walk_urdf_chain(in), Exception, match);
}

BOOST_AUTO_TEST_CASE(to_row_each_type) {
    const std::vector<std::pair<std::string, JointType>> cases{
        {"revolute", JointType::revolute},
        {"continuous", JointType::continuous},
        {"prismatic", JointType::prismatic},
        {"fixed", JointType::fixed},
    };
    for (const auto& c : cases) {
        ParsedJoint p = mk("j", "a", "b", c.first);
        p.axis_opt = Vector3{0, 0, 1};
        BOOST_CHECK(to_row(p).type == c.second);
    }
}

BOOST_AUTO_TEST_CASE(to_row_fixed_zeroes_axis) {
    ParsedJoint p = mk("j", "a", "b", "fixed");
    p.axis_opt = Vector3{1, 2, 3};
    auto row = to_row(p);
    BOOST_CHECK(row.axis == Vector3{});
}

BOOST_AUTO_TEST_CASE(to_row_default_axis_for_nonfixed) {
    ParsedJoint p = mk("j", "a", "b", "revolute");  // no axis_opt
    auto row = to_row(p);
    BOOST_CHECK_CLOSE(row.axis.x(), 1.0, 1e-9);
    BOOST_CHECK_SMALL(row.axis.y(), 1e-9);
    BOOST_CHECK_SMALL(row.axis.z(), 1e-9);
}

BOOST_AUTO_TEST_CASE(to_row_unsupported_type_throws) {
    ParsedJoint p = mk("jf", "a", "b", "floating");
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("unsupported type 'floating'") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(to_row(p), Exception, match);
}

BOOST_AUTO_TEST_CASE(to_row_zero_axis_throws) {
    ParsedJoint p = mk("j", "a", "b", "revolute");
    p.axis_opt = Vector3{0, 0, 0};
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("zero-magnitude axis") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(to_row(p), Exception, match);
}

BOOST_AUTO_TEST_CASE(kinematics_to_model_table_end_to_end_inline) {
    const std::string xml = R"(<?xml version="1.0"?>
<robot name="r">
  <link name="base"/><link name="l1"/><link name="tool"/>
  <joint name="j1" type="revolute">
    <parent link="base"/><child link="l1"/>
    <origin xyz="0 0 0.5"/><axis xyz="0 0 1"/>
  </joint>
  <joint name="fix" type="fixed">
    <parent link="l1"/><child link="tool"/>
    <origin xyz="0.1 0 0"/>
  </joint>
</robot>)";
    auto table = kinematics_to_model_table(urdf_from_string(xml));
    BOOST_REQUIRE_EQUAL(table.size(), 2u);

    BOOST_CHECK_EQUAL(table[0].name, "j1");
    BOOST_CHECK(table[0].type == JointType::revolute);
    BOOST_CHECK_CLOSE(table[0].xyz.z(), 0.5, 1e-9);
    BOOST_CHECK_CLOSE(table[0].axis.z(), 1.0, 1e-9);

    BOOST_CHECK_EQUAL(table[1].name, "fix");
    BOOST_CHECK(table[1].type == JointType::fixed);
    BOOST_CHECK(table[1].axis == Vector3{});
}

BOOST_AUTO_TEST_CASE(tensor_shape_and_dtype) {
    std::vector<JointRow> table{
        JointRow{"a", {1, 2, 3}, {0.1, 0.2, 0.3}, {0, 0, 1}, JointType::revolute},
        JointRow{"b", {4, 5, 6}, {0, 0, 0}, {0, 0, 0}, JointType::fixed},
    };
    auto t = model_table_to_tensor(table);
    BOOST_CHECK_EQUAL(t.dimension(), 2u);
    BOOST_CHECK_EQUAL(t.shape()[0], 2u);
    BOOST_CHECK_EQUAL(t.shape()[1], 10u);
    static_assert(std::is_same<decltype(t)::value_type, double>::value, "dtype must be double");
}

BOOST_AUTO_TEST_CASE(tensor_geometry_columns) {
    std::vector<JointRow> table{
        JointRow{"a", {1.5, 2.5, 3.5}, {0.1, 0.2, 0.3}, {0.4, 0.5, 0.6}, JointType::revolute},
    };
    auto t = model_table_to_tensor(table);
    BOOST_CHECK_CLOSE(t(0, 0), 1.5, 1e-9);
    BOOST_CHECK_CLOSE(t(0, 1), 2.5, 1e-9);
    BOOST_CHECK_CLOSE(t(0, 2), 3.5, 1e-9);
    BOOST_CHECK_CLOSE(t(0, 3), 0.1, 1e-9);
    BOOST_CHECK_CLOSE(t(0, 6), 0.4, 1e-9);
    BOOST_CHECK_CLOSE(t(0, 8), 0.6, 1e-9);
}

BOOST_AUTO_TEST_CASE(tensor_type_column) {
    const Vector3 z{};
    std::vector<JointRow> table{
        JointRow{"a", z, z, {0, 0, 1}, JointType::revolute},
        JointRow{"b", z, z, {0, 0, 1}, JointType::continuous},
        JointRow{"c", z, z, {1, 0, 0}, JointType::prismatic},
        JointRow{"d", z, z, {0, 0, 0}, JointType::fixed},
    };
    auto t = model_table_to_tensor(table);
    BOOST_CHECK_EQUAL(t(0, 9), 0.0);  // revolute
    BOOST_CHECK_EQUAL(t(1, 9), 1.0);  // continuous
    BOOST_CHECK_EQUAL(t(2, 9), 2.0);  // prismatic
    BOOST_CHECK_EQUAL(t(3, 9), 3.0);  // fixed
}

BOOST_AUTO_TEST_CASE(tensor_fixed_joint_axis_zero) {
    std::vector<JointRow> table{
        JointRow{"f", {0.1, 0.2, 0.3}, {0, 0, 0}, {0, 0, 0}, JointType::fixed},
    };
    auto t = model_table_to_tensor(table);
    BOOST_CHECK_EQUAL(t(0, 6), 0.0);
    BOOST_CHECK_EQUAL(t(0, 7), 0.0);
    BOOST_CHECK_EQUAL(t(0, 8), 0.0);
    BOOST_CHECK_EQUAL(t(0, 9), 3.0);  // fixed encoded as 3
}

BOOST_AUTO_TEST_CASE(model_table_to_tensor_empty_throws) {
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("empty model table") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(model_table_to_tensor({}), Exception, match);
}

BOOST_AUTO_TEST_CASE(tensor_to_model_table_round_trip) {
    std::vector<JointRow> table{
        JointRow{"a", {1.5, 2.5, 3.5}, {0.1, 0.2, 0.3}, {0.4, 0.5, 0.6}, JointType::revolute},
        JointRow{"b", {4, 5, 6}, {0, 0, 0}, {1, 0, 0}, JointType::prismatic},
        JointRow{"c", {7, 8, 9}, {0, 0, 0}, {0, 0, 0}, JointType::fixed},
    };
    auto t = model_table_to_tensor(table);
    auto rebuilt = tensor_to_model_table(t);

    BOOST_REQUIRE_EQUAL(rebuilt.size(), table.size());
    for (std::size_t i = 0; i < table.size(); ++i) {
        BOOST_CHECK(rebuilt[i].xyz == table[i].xyz);
        BOOST_CHECK(rebuilt[i].rpy == table[i].rpy);
        BOOST_CHECK(rebuilt[i].axis == table[i].axis);
        BOOST_CHECK(rebuilt[i].type == table[i].type);
        BOOST_CHECK(rebuilt[i].name.empty());  // names are not carried in the tensor
    }
}

BOOST_AUTO_TEST_CASE(tensor_to_model_table_wrong_dim_throws) {
    xt::xarray<double> t1d = xt::zeros<double>({std::size_t{10}});
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("expected 2D tensor") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(tensor_to_model_table(t1d), Exception, match);
}

BOOST_AUTO_TEST_CASE(tensor_to_model_table_wrong_cols_throws) {
    xt::xarray<double> bad = xt::zeros<double>({std::size_t{3}, std::size_t{5}});
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("expected shape (n, 10)") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(tensor_to_model_table(bad), Exception, match);
}

BOOST_AUTO_TEST_CASE(tensor_to_model_table_empty_throws) {
    xt::xarray<double> empty = xt::zeros<double>({std::size_t{0}, std::size_t{10}});
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("empty tensor") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(tensor_to_model_table(empty), Exception, match);
}

BOOST_AUTO_TEST_CASE(tensor_to_model_table_fractional_type_throws) {
    xt::xarray<double> t = xt::zeros<double>({std::size_t{1}, std::size_t{10}});
    t(0, 9) = 0.5;  // non-integer joint type
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("invalid joint type value") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(tensor_to_model_table(t), Exception, match);
}

BOOST_AUTO_TEST_CASE(tensor_to_model_table_out_of_range_type_throws) {
    xt::xarray<double> t = xt::zeros<double>({std::size_t{1}, std::size_t{10}});
    t(0, 9) = 7.0;  // outside [0, 3]
    auto match = [](const Exception& e) {
        return std::string(e.what()).find("invalid joint type value") != std::string::npos;
    };
    BOOST_CHECK_EXCEPTION(tensor_to_model_table(t), Exception, match);
}

namespace {
KinematicsDataURDF load_urdf(const std::string& filename) {
    std::ifstream f(std::string(VIAMCPPSDK_TEST_TESTFILES_DIR) + "/" + filename, std::ios::binary);
    if (!f)
        throw std::runtime_error("failed to open " + filename);
    std::ostringstream buf;
    buf << f.rdbuf();
    return urdf_from_string(buf.str());
}
}  // namespace

BOOST_AUTO_TEST_CASE(model_table_gp12_has_fixed_tool0) {
    auto table = kinematics_to_model_table(load_urdf("gp12.urdf"));
    bool has_fixed_tool = false;
    for (const auto& row : table) {
        if (row.type == JointType::fixed) {
            has_fixed_tool = true;
            BOOST_CHECK(row.axis == Vector3{});
        }
    }
    BOOST_CHECK(has_fixed_tool);
}

BOOST_AUTO_TEST_CASE(tensor_round_trip_gp12) {
    auto table = kinematics_to_model_table(load_urdf("gp12.urdf"));
    auto t = model_table_to_tensor(table);
    BOOST_CHECK_EQUAL(t.shape()[0], table.size());
    BOOST_CHECK_EQUAL(t.shape()[1], 10u);
    for (std::size_t i = 0; i < table.size(); ++i) {
        const double type_v = t(i, 9);
        BOOST_CHECK(type_v >= 0.0 && type_v <= 3.0);
        BOOST_CHECK_EQUAL(type_v, static_cast<double>(static_cast<int>(type_v)));
    }
}

BOOST_AUTO_TEST_CASE(model_table_gp12_full_chain) {
    auto table = kinematics_to_model_table(load_urdf("gp12.urdf"));

    // 6 revolute joints + 1 fixed tool joint.
    BOOST_REQUIRE_EQUAL(table.size(), 7u);

    // Chain order: 6 revolute joints in URDF order, then the fixed tool joint.
    BOOST_CHECK_EQUAL(table[0].name, "joint_1_s");
    BOOST_CHECK_EQUAL(table[1].name, "joint_2_l");
    BOOST_CHECK_EQUAL(table[2].name, "joint_3_u");
    BOOST_CHECK_EQUAL(table[3].name, "joint_4_r");
    BOOST_CHECK_EQUAL(table[4].name, "joint_5_b");
    BOOST_CHECK_EQUAL(table[5].name, "joint_6_t");
    BOOST_CHECK_EQUAL(table[6].name, "joint_6_t-tool0");

    // Types: 6 revolute then 1 fixed.
    for (std::size_t i = 0; i < 6; ++i) {
        BOOST_CHECK(table[i].type == JointType::revolute);
    }
    BOOST_CHECK(table[6].type == JointType::fixed);

    // Joint 1: shoulder, vertical axis at z=0.450.
    BOOST_CHECK_SMALL(table[0].xyz.x(), 1e-9);
    BOOST_CHECK_SMALL(table[0].xyz.y(), 1e-9);
    BOOST_CHECK_CLOSE(table[0].xyz.z(), 0.450, 1e-9);
    BOOST_CHECK_SMALL(table[0].axis.x(), 1e-9);
    BOOST_CHECK_SMALL(table[0].axis.y(), 1e-9);
    BOOST_CHECK_CLOSE(table[0].axis.z(), 1.0, 1e-9);

    // Joint 4: elbow offset (0.640, 0, 0.200), axis=(-1, 0, 0).
    BOOST_CHECK_CLOSE(table[3].xyz.x(), 0.640, 1e-9);
    BOOST_CHECK_CLOSE(table[3].xyz.z(), 0.200, 1e-9);
    BOOST_CHECK_CLOSE(table[3].axis.x(), -1.0, 1e-9);

    // Fixed tool joint: nonzero rpy, zero axis (substituted by to_row).
    BOOST_CHECK_CLOSE(table[6].xyz.x(), 0.100, 1e-9);
    BOOST_CHECK_CLOSE(table[6].rpy.y(), -1.570796, 1e-3);
    BOOST_CHECK(table[6].axis == Vector3{});

    // Tensor cross-check: same 7 rows, 10 columns, type column at index 9.
    auto t = model_table_to_tensor(table);
    BOOST_REQUIRE_EQUAL(t.shape()[0], 7u);
    BOOST_REQUIRE_EQUAL(t.shape()[1], 10u);
    BOOST_CHECK_EQUAL(t(6, 9), 3.0);          // tool0 is fixed (enum value 3)
    BOOST_CHECK_EQUAL(t(0, 9), 0.0);          // joint_1_s is revolute (enum value 0)
    BOOST_CHECK_CLOSE(t(0, 2), 0.450, 1e-9);  // joint_1_s z
}

BOOST_AUTO_TEST_CASE(parse_urdf_gp12_full_file) {
    auto parsed = parse_urdf(load_urdf("gp12.urdf"));

    BOOST_REQUIRE_EQUAL(parsed.size(), 7u);

    // Joint 0: joint_1_s
    BOOST_CHECK_EQUAL(parsed[0].name, "joint_1_s");
    BOOST_CHECK_EQUAL(parsed[0].type_str, "revolute");
    BOOST_CHECK_EQUAL(parsed[0].parent_link, "base_link");
    BOOST_CHECK_EQUAL(parsed[0].child_link, "link_1_s");
    BOOST_CHECK_SMALL(parsed[0].xyz.x(), 1e-9);
    BOOST_CHECK_SMALL(parsed[0].xyz.y(), 1e-9);
    BOOST_CHECK_CLOSE(parsed[0].xyz.z(), 0.450, 1e-9);
    BOOST_CHECK(parsed[0].rpy == Vector3{});
    BOOST_REQUIRE(parsed[0].axis_opt.has_value());
    BOOST_CHECK_SMALL(parsed[0].axis_opt->x(), 1e-9);
    BOOST_CHECK_SMALL(parsed[0].axis_opt->y(), 1e-9);
    BOOST_CHECK_CLOSE(parsed[0].axis_opt->z(), 1.0, 1e-9);

    // Joint 1: joint_2_l
    BOOST_CHECK_EQUAL(parsed[1].name, "joint_2_l");
    BOOST_CHECK_EQUAL(parsed[1].type_str, "revolute");
    BOOST_CHECK_EQUAL(parsed[1].parent_link, "link_1_s");
    BOOST_CHECK_EQUAL(parsed[1].child_link, "link_2_l");
    BOOST_CHECK_CLOSE(parsed[1].xyz.x(), 0.155, 1e-9);
    BOOST_CHECK_SMALL(parsed[1].xyz.y(), 1e-9);
    BOOST_CHECK_SMALL(parsed[1].xyz.z(), 1e-9);
    BOOST_REQUIRE(parsed[1].axis_opt.has_value());
    BOOST_CHECK_CLOSE(parsed[1].axis_opt->y(), 1.0, 1e-9);

    // Joint 2: joint_3_u
    BOOST_CHECK_EQUAL(parsed[2].name, "joint_3_u");
    BOOST_CHECK_EQUAL(parsed[2].type_str, "revolute");
    BOOST_CHECK_EQUAL(parsed[2].parent_link, "link_2_l");
    BOOST_CHECK_EQUAL(parsed[2].child_link, "link_3_u");
    BOOST_CHECK_CLOSE(parsed[2].xyz.z(), 0.614, 1e-9);
    BOOST_REQUIRE(parsed[2].axis_opt.has_value());
    BOOST_CHECK_CLOSE(parsed[2].axis_opt->y(), -1.0, 1e-9);

    // Joint 3: joint_4_r
    BOOST_CHECK_EQUAL(parsed[3].name, "joint_4_r");
    BOOST_CHECK_EQUAL(parsed[3].type_str, "revolute");
    BOOST_CHECK_EQUAL(parsed[3].parent_link, "link_3_u");
    BOOST_CHECK_EQUAL(parsed[3].child_link, "link_4_r");
    BOOST_CHECK_CLOSE(parsed[3].xyz.x(), 0.640, 1e-9);
    BOOST_CHECK_CLOSE(parsed[3].xyz.z(), 0.200, 1e-9);
    BOOST_REQUIRE(parsed[3].axis_opt.has_value());
    BOOST_CHECK_CLOSE(parsed[3].axis_opt->x(), -1.0, 1e-9);

    // Joint 4: joint_5_b
    BOOST_CHECK_EQUAL(parsed[4].name, "joint_5_b");
    BOOST_CHECK_EQUAL(parsed[4].type_str, "revolute");
    BOOST_CHECK_EQUAL(parsed[4].parent_link, "link_4_r");
    BOOST_CHECK_EQUAL(parsed[4].child_link, "link_5_b");
    BOOST_CHECK(parsed[4].xyz == Vector3{});
    BOOST_REQUIRE(parsed[4].axis_opt.has_value());
    BOOST_CHECK_CLOSE(parsed[4].axis_opt->y(), -1.0, 1e-9);

    // Joint 5: joint_6_t
    BOOST_CHECK_EQUAL(parsed[5].name, "joint_6_t");
    BOOST_CHECK_EQUAL(parsed[5].type_str, "revolute");
    BOOST_CHECK_EQUAL(parsed[5].parent_link, "link_5_b");
    BOOST_CHECK_EQUAL(parsed[5].child_link, "link_6_t");
    BOOST_CHECK(parsed[5].xyz == Vector3{});
    BOOST_REQUIRE(parsed[5].axis_opt.has_value());
    BOOST_CHECK_CLOSE(parsed[5].axis_opt->x(), -1.0, 1e-9);

    // Joint 6: joint_6_t-tool0 (fixed, no <axis> element)
    BOOST_CHECK_EQUAL(parsed[6].name, "joint_6_t-tool0");
    BOOST_CHECK_EQUAL(parsed[6].type_str, "fixed");
    BOOST_CHECK_EQUAL(parsed[6].parent_link, "link_6_t");
    BOOST_CHECK_EQUAL(parsed[6].child_link, "tool0");
    BOOST_CHECK_CLOSE(parsed[6].xyz.x(), 0.100, 1e-9);
    BOOST_CHECK_SMALL(parsed[6].xyz.y(), 1e-9);
    BOOST_CHECK_SMALL(parsed[6].xyz.z(), 1e-9);
    BOOST_CHECK_CLOSE(parsed[6].rpy.x(), 3.1415926, 1e-3);
    BOOST_CHECK_CLOSE(parsed[6].rpy.y(), -1.570796, 1e-3);
    BOOST_CHECK_SMALL(parsed[6].rpy.z(), 1e-9);
    BOOST_CHECK(!parsed[6].axis_opt.has_value());  // fixed joint has no <axis> in URDF
}

BOOST_AUTO_TEST_CASE(tensor_to_model_table_gp12_round_trip) {
    auto table = kinematics_to_model_table(load_urdf("gp12.urdf"));
    auto t = model_table_to_tensor(table);
    auto rebuilt = tensor_to_model_table(t);
    BOOST_REQUIRE_EQUAL(rebuilt.size(), table.size());
    for (std::size_t i = 0; i < table.size(); ++i) {
        BOOST_CHECK(rebuilt[i].xyz == table[i].xyz);
        BOOST_CHECK(rebuilt[i].rpy == table[i].rpy);
        BOOST_CHECK(rebuilt[i].axis == table[i].axis);
        BOOST_CHECK(rebuilt[i].type == table[i].type);
    }
}
