#define BOOST_TEST_MODULE test module test_kinematic_model

#include <cmath>
#include <fstream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <boost/test/included/unit_test.hpp>
#include <boost/variant/get.hpp>

#include <google/protobuf/struct.pb.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/message_differencer.h>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/kinematic_model_file.hpp>
#include <viam/sdk/common/private/kinematic_model_file.hpp>
#include <viam/sdk/common/private/kinematics_response.hpp>

namespace viam {
namespace sdktests {

using namespace viam::sdk;

namespace {

const std::string k_fixture_dir = VIAMCPPSDK_TEST_KINEMATIC_MODELS_DIR;

// the case directories, listed by cmake since c++14 has no way to walk a directory
std::vector<std::string> fixture_cases() {
    std::vector<std::string> cases;
    std::istringstream names(VIAMCPPSDK_TEST_KINEMATIC_MODEL_CASES);
    for (std::string name; std::getline(names, name, ',');) {
        if (!name.empty()) {
            cases.push_back(name);
        }
    }
    return cases;
}

bool read_file(const std::string& path, std::string* out) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return false;
    }
    std::ostringstream contents;
    contents << file.rdbuf();
    *out = contents.str();
    return true;
}

// The error kinds the fixtures name, from the README beside them, with what our loader says for
// each. The two parser kinds have no wording of ours to look for, since the text is protobuf's
// and changes between protobuf versions, so they only check that the parse is what failed.
const std::map<std::string, std::string>& error_wording() {
    static const std::map<std::string, std::string> wording = {
        {"unknown_field", "not the proto JSON of a kinematic model"},
        {"malformed_json", "not the proto JSON of a kinematic model"},
        {"infinite", "is infinite"},
        {"nan", "is not a number"},
        {"empty_model", "no links or joints"},
        {"missing_id", "has no id"},
        {"reserved_id", "reserved id"},
        {"shared_id", "names both a link and a joint"},
        {"duplicate_id", "is defined twice"},
        {"missing_parent", "is not a link or joint"},
        {"cycle", "its own ancestor"},
        {"mimic_self", "mimics itself"},
        {"mimic_not_joint", "which is not a joint"},
        {"mimic_with_limits", "cannot carry limits of its own"},
        {"mesh_missing", "failed to read mesh"},
        {"mesh_path_absolute", "is absolute"},
        {"mesh_path_uri", "is a URI"},
        {"mesh_path_backslash", "uses backslashes"},
        {"content_type_mismatch", "disagrees"},
        {"collision_format", "is not stl or ply"},
        {"mesh_no_extension", "has no extension"},
        {"inline_without_type", "need a content type"},
        {"mesh_empty", "neither bytes nor a source path"},
    };
    return wording;
}

void check_error_case(const std::string& model_path, const std::string& error_json) {
    google::protobuf::Struct want;
    BOOST_REQUIRE(google::protobuf::util::JsonStringToMessage(error_json, &want).ok());
    const std::string kind = want.fields().at("kind").string_value();
    const auto says = error_wording().find(kind);
    BOOST_REQUIRE_MESSAGE(says != error_wording().end(), "unknown error kind " << kind);

    std::string message;
    try {
        impl::read_kinematic_model_file_message(model_path);
        BOOST_ERROR("the loader accepted a file it has to refuse as " << kind);
        return;
    } catch (const Exception& xcp) {
        message = xcp.what();
    }
    BOOST_CHECK_MESSAGE(
        message.find(says->second) != std::string::npos,
        "error for " << kind << " does not say \"" << says->second << "\": " << message);
    for (const auto& mention : want.fields().at("mentions").list_value().values()) {
        BOOST_CHECK_MESSAGE(
            message.find(mention.string_value()) != std::string::npos,
            "error does not mention \"" << mention.string_value() << "\": " << message);
    }

    // the public entry point refuses it the same way
    BOOST_CHECK_THROW(read_kinematic_model_file(model_path), Exception);
}

void check_expected_case(const std::string& model_path, const std::string& expected_json) {
    common::v1::KinematicModel expected;
    const auto status = google::protobuf::util::JsonStringToMessage(expected_json, &expected);
    BOOST_REQUIRE_MESSAGE(status.ok(), status.ToString());

    const common::v1::KinematicModel loaded = impl::read_kinematic_model_file_message(model_path);
    google::protobuf::util::MessageDifferencer differencer;
    std::string difference;
    differencer.ReportDifferencesToString(&difference);
    BOOST_CHECK_MESSAGE(differencer.Compare(expected, loaded), difference);

    // What a module holds is the sdk type, so going through it must not lose anything a second
    // trip would notice, and it has to keep every link, joint and mesh byte of the message.
    const KinematicModel model = read_kinematic_model_file(model_path);
    BOOST_CHECK(model == from_proto(to_proto(model)));
    BOOST_CHECK_EQUAL(model.name, expected.name());
    BOOST_REQUIRE_EQUAL(model.links.size(), static_cast<std::size_t>(expected.links_size()));
    BOOST_REQUIRE_EQUAL(model.joints.size(), static_cast<std::size_t>(expected.joints_size()));
    const common::v1::KinematicModel served = to_proto(model);
    for (int i = 0; i < expected.links_size(); ++i) {
        // the sdk type always holds a pose, where the message can leave one out, so the pose
        // is the one field that does not come back as it was written
        common::v1::KinematicLink want = expected.links(i);
        common::v1::KinematicLink got = served.links(i);
        want.clear_pose();
        got.clear_pose();
        BOOST_CHECK_MESSAGE(differencer.Compare(want, got), difference);
    }
    for (int i = 0; i < expected.joints_size(); ++i) {
        // an empty limits message says the same as none, and the sdk type holds only the bounds
        common::v1::KinematicJoint want = expected.joints(i);
        if (want.limits().ByteSizeLong() == 0) {
            want.clear_limits();
        }
        BOOST_CHECK_MESSAGE(differencer.Compare(want, served.joints(i)), difference);
    }
}

}  // namespace

BOOST_AUTO_TEST_SUITE(test_kinematic_model_file)

// The fixtures are a copy of rdk's referenceframe/testfiles/kinematic_models, the contract every
// sdk's loader is held to. A case with an expected.json has to load into exactly that message,
// mesh bytes and all, and a case with an error.json has to be refused for the reason it names.
BOOST_AUTO_TEST_CASE(fixtures) {
    const std::vector<std::string> cases = fixture_cases();
    BOOST_REQUIRE_GT(cases.size(), 20U);
    for (const auto& name : cases) {
        BOOST_TEST_CONTEXT(name) {
            const std::string dir = k_fixture_dir + "/" + name;
            const std::string model_path = dir + "/model.json";
            std::string contents;
            if (read_file(dir + "/error.json", &contents)) {
                check_error_case(model_path, contents);
            } else if (read_file(dir + "/expected.json", &contents)) {
                check_expected_case(model_path, contents);
            } else {
                BOOST_ERROR("case has neither expected.json nor error.json");
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(missing_file) {
    BOOST_CHECK_THROW(read_kinematic_model_file(k_fixture_dir + "/no_such_case/model.json"),
                      Exception);
}

BOOST_AUTO_TEST_SUITE_END()

namespace {

// a revolute joint with every bound, a prismatic joint with only a position range, a continuous
// joint, and a joint that mimics the first
KinematicModel limits_model() {
    KinematicModel model;
    model.name = "arm";

    KinematicLink base;
    base.id = "base";

    KinematicJoint shoulder;
    shoulder.id = "shoulder";
    shoulder.parent = "base";
    shoulder.axis = Vector3{0, 0, 1};
    shoulder.spec = RevoluteJoint{-180.0, 180.0, 90.0, 400.0};

    KinematicJoint rail;
    rail.id = "rail";
    rail.parent = "base";
    rail.axis = Vector3{1, 0, 0};
    rail.spec = PrismaticJoint{0.0, 500.0, boost::none, boost::none, boost::none};

    KinematicJoint wrist;
    wrist.id = "wrist";
    wrist.parent = "base";
    wrist.axis = Vector3{0, 0, 1};
    wrist.spec = ContinuousJoint{180.0, boost::none};

    KinematicJoint follower;
    follower.id = "follower";
    follower.parent = "base";
    follower.axis = Vector3{0, 0, 1};
    follower.spec = RevoluteJoint{};
    follower.mimic = MimicJoint{"shoulder", 1.0, 0.0};

    model.links = {base};
    model.joints = {shoulder, rail, wrist, follower};
    return model;
}

JointLimits make_limits(boost::optional<double> min,
                        boost::optional<double> max,
                        boost::optional<double> max_velocity,
                        boost::optional<double> max_acceleration) {
    JointLimits limits;
    limits.min = min;
    limits.max = max;
    limits.max_velocity = max_velocity;
    limits.max_acceleration = max_acceleration;
    return limits;
}

// narrowing has to fail naming the joint and leave the model as it was
void check_refused(const std::map<std::string, JointLimits>& limits, const std::string& mention) {
    KinematicModel model = limits_model();
    try {
        narrow_joint_limits(model, limits);
        BOOST_ERROR("narrow_joint_limits accepted limits it has to refuse");
    } catch (const Exception& xcp) {
        const std::string message = xcp.what();
        BOOST_CHECK_MESSAGE(message.find(mention) != std::string::npos,
                            "error does not mention \"" << mention << "\": " << message);
    }
    BOOST_CHECK(model == limits_model());
}

}  // namespace

BOOST_AUTO_TEST_SUITE(test_narrow_joint_limits)

BOOST_AUTO_TEST_CASE(narrows_each_limit_on_its_own) {
    KinematicModel model = limits_model();
    narrow_joint_limits(model,
                        {{"shoulder", make_limits(-31.0, boost::none, 30.0, boost::none)},
                         {"rail", make_limits(boost::none, 250.0, 100.0, 50.0)}});

    BOOST_CHECK(model.joints[0].spec == joint_spec(RevoluteJoint{-31.0, 180.0, 30.0, 400.0}));
    // the rail had no speed bounds, so any value narrows it
    BOOST_CHECK(model.joints[1].spec ==
                joint_spec(PrismaticJoint{0.0, 250.0, 100.0, 50.0, boost::none}));
    BOOST_CHECK(model.joints[2] == limits_model().joints[2]);
}

BOOST_AUTO_TEST_CASE(equal_bounds_and_locked_joint) {
    KinematicModel model = limits_model();
    narrow_joint_limits(model, {{"shoulder", make_limits(-180.0, 180.0, 90.0, 400.0)}});
    BOOST_CHECK(model == limits_model());

    narrow_joint_limits(model, {{"shoulder", make_limits(45.0, 45.0, boost::none, boost::none)}});
    BOOST_CHECK(model.joints[0].spec == joint_spec(RevoluteJoint{45.0, 45.0, 90.0, 400.0}));
}

BOOST_AUTO_TEST_CASE(continuous_joint) {
    KinematicModel model = limits_model();
    narrow_joint_limits(model, {{"wrist", make_limits(boost::none, boost::none, 90.0, 300.0)}});
    BOOST_CHECK(model.joints[2].spec == joint_spec(ContinuousJoint{90.0, 300.0}));

    // a position bound is what makes it a revolute joint
    narrow_joint_limits(model,
                        {{"wrist", make_limits(-90.0, boost::none, boost::none, boost::none)}});
    BOOST_CHECK(model.joints[2].spec == joint_spec(RevoluteJoint{-90.0, boost::none, 90.0, 300.0}));
}

BOOST_AUTO_TEST_CASE(nothing_requested_changes_nothing) {
    KinematicModel model = limits_model();
    narrow_joint_limits(model, {});
    narrow_joint_limits(model, {{"shoulder", JointLimits{}}});
    BOOST_CHECK(model == limits_model());
}

BOOST_AUTO_TEST_CASE(refusals) {
    const boost::optional<double> none;
    check_refused({{"shoulder", make_limits(-181.0, none, none, none)}}, "below the model's min");
    check_refused({{"shoulder", make_limits(none, 180.5, none, none)}}, "above the model's max");
    check_refused({{"shoulder", make_limits(none, none, 91.0, none)}}, "max_velocity");
    check_refused({{"shoulder", make_limits(none, none, none, 401.0)}}, "max_acceleration");
    check_refused({{"shoulder", make_limits(10.0, -10.0, none, none)}}, "is above max");
    check_refused({{"shoulder", make_limits(none, none, -1.0, none)}}, "cannot be negative");
    check_refused(
        {{"rail", make_limits(none, std::numeric_limits<double>::infinity(), none, none)}},
        "finite");
    check_refused({{"rail", make_limits(std::nan(""), none, none, none)}}, "finite");
    check_refused({{"elbow", make_limits(none, none, 10.0, none)}}, "\"elbow\"");
    check_refused({{"follower", make_limits(none, none, 10.0, none)}}, "mimic");

    // a value a hair outside a limit is printed with enough digits to differ from the limit
    check_refused({{"shoulder", make_limits(none, 180.0000001, none, none)}},
                  "max 180.0000001 is above");

    // a good request beside a bad one does not land either
    check_refused({{"rail", make_limits(none, 250.0, none, none)},
                   {"shoulder", make_limits(none, 190.0, none, none)}},
                  "\"shoulder\"");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_kinematics_response)

namespace {

KinematicModel mesh_model() {
    KinematicLink link;
    link.id = "base";
    link.collision = {GeometryConfig(pose{}, mesh{"stl", {1, 2, 3}, std::string("c.stl")}, "c")};
    link.visual = {GeometryConfig(pose{}, mesh{"glb", {4, 5}, std::string("v.glb")}, "v")};
    KinematicModel model;
    model.name = "name_in_the_file";
    model.links = {link};
    return model;
}

}  // namespace

BOOST_AUTO_TEST_CASE(named_for_the_resource_with_meshes_as_returned) {
    common::v1::GetKinematicsResponse response;
    impl::set_response_model(mesh_model(), "resource", &response);
    BOOST_CHECK_EQUAL(response.model().name(), "resource");
    // leaving bytes out is the component's job, so the server sends what it was handed
    const auto& link = response.model().links(0);
    BOOST_CHECK_EQUAL(link.collision(0).mesh().mesh().size(), 3U);
    BOOST_CHECK_EQUAL(link.visual(0).mesh().mesh().size(), 2U);
}

namespace {

std::size_t mesh_size(const GeometryConfig& geometry) {
    return boost::get<mesh>(geometry.get_geometry_specifics()).data.size();
}

KinematicModel dropped(bool collision, bool visual) {
    KinematicModel model = mesh_model();
    KinematicJoint rail;
    rail.id = "rail";
    rail.parent = "base";
    PrismaticJoint spec;
    spec.geometry = GeometryConfig(pose{}, mesh{"ply", {6}, std::string("r.ply")}, "r");
    rail.spec = spec;
    model.joints = {rail};
    drop_unrequested_mesh_bytes(model, kinematics_options{collision, visual});
    return model;
}

std::size_t rail_mesh_size(const KinematicModel& model) {
    return mesh_size(*boost::get<PrismaticJoint>(model.joints[0].spec).geometry);
}

}  // namespace

BOOST_AUTO_TEST_CASE(drop_unrequested_mesh_bytes_follows_the_flags) {
    // a request with neither flag may predate them, so the model stays whole
    const KinematicModel whole = dropped(false, false);
    BOOST_CHECK_EQUAL(mesh_size(whole.links[0].collision[0]), 3U);
    BOOST_CHECK_EQUAL(mesh_size(whole.links[0].visual[0]), 2U);
    BOOST_CHECK_EQUAL(rail_mesh_size(whole), 1U);

    const KinematicModel visual_only = dropped(false, true);
    const GeometryConfig& collision = visual_only.links[0].collision[0];
    BOOST_CHECK_EQUAL(mesh_size(collision), 0U);
    BOOST_CHECK_EQUAL(mesh_size(visual_only.links[0].visual[0]), 2U);
    BOOST_CHECK_EQUAL(rail_mesh_size(visual_only), 0U);
    // everything but the bytes survives
    const mesh& kept = boost::get<mesh>(collision.get_geometry_specifics());
    BOOST_CHECK_EQUAL(kept.content_type, "stl");
    BOOST_CHECK(kept.original_file_source_path_diagnostic == std::string("c.stl"));
    BOOST_CHECK_EQUAL(collision.get_label(), "c");

    const KinematicModel collision_only = dropped(true, false);
    BOOST_CHECK_EQUAL(mesh_size(collision_only.links[0].collision[0]), 3U);
    BOOST_CHECK_EQUAL(mesh_size(collision_only.links[0].visual[0]), 0U);
    BOOST_CHECK_EQUAL(rail_mesh_size(collision_only), 1U);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
