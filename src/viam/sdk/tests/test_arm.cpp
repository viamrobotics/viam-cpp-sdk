#define BOOST_TEST_MODULE test module test_arm
#include <viam/sdk/components/arm.hpp>

#include <boost/optional/optional_io.hpp>
#include <boost/qvm/all.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>

#include <stdexcept>
#include <thread>

#include <grpcpp/support/status.h>

#include <viam/api/component/arm/v1/arm.grpc.pb.h>
#include <viam/api/component/arm/v1/arm.pb.h>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/tests/mocks/mock_arm.hpp>
#include <viam/sdk/tests/test_utils.hpp>

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<viam::sdk::GeometryConfig>)
BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<double>)
BOOST_TEST_DONT_PRINT_LOG_VALUE(std::vector<std::vector<double>>)
BOOST_TEST_DONT_PRINT_LOG_VALUE(viam::sdk::KinematicsData)

namespace viam {
namespace sdktests {

using namespace arm;

using namespace viam::sdk;

struct CheckScalar : boost::static_visitor<> {
    double expected;
    explicit CheckScalar(double e) : expected(e) {}
    void operator()(double d) const {
        BOOST_CHECK_EQUAL(d, expected);
    }
    void operator()(const std::vector<double>&) const {
        BOOST_FAIL("Expected scalar, got vector");
    }
};

struct CheckVector : boost::static_visitor<> {
    std::vector<double> expected;
    explicit CheckVector(std::vector<double> e) : expected(std::move(e)) {}
    void operator()(double) const {
        BOOST_FAIL("Expected vector, got scalar");
    }
    void operator()(const std::vector<double>& v) const {
        BOOST_CHECK_EQUAL(v, expected);
    }
};

namespace {

// Builds a trajectory point. Passing velocities engages the constraints;
// passing accelerations too engages those within.
Arm::trajectory_point make_point(std::int64_t time_us,
                                 std::vector<double> positions,
                                 boost::optional<std::vector<double>> velocities = boost::none,
                                 boost::optional<std::vector<double>> accelerations = boost::none) {
    Arm::trajectory_point point;
    point.time = std::chrono::microseconds(time_us);
    point.positions = std::move(positions);
    if (velocities) {
        Arm::trajectory_point::kinematic_constraints constraints;
        constraints.velocities = std::move(*velocities);
        constraints.accelerations = std::move(accelerations);
        point.constraints = std::move(constraints);
    }
    return point;
}

void check_points_equal(const Arm::trajectory_point& got, const Arm::trajectory_point& want) {
    BOOST_CHECK(got.time == want.time);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        got.positions.begin(), got.positions.end(), want.positions.begin(), want.positions.end());
    BOOST_REQUIRE_EQUAL(static_cast<bool>(got.constraints), static_cast<bool>(want.constraints));
    if (!want.constraints) {
        return;
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(got.constraints->velocities.begin(),
                                  got.constraints->velocities.end(),
                                  want.constraints->velocities.begin(),
                                  want.constraints->velocities.end());
    BOOST_REQUIRE_EQUAL(static_cast<bool>(got.constraints->accelerations),
                        static_cast<bool>(want.constraints->accelerations));
    if (want.constraints->accelerations) {
        BOOST_CHECK_EQUAL_COLLECTIONS(got.constraints->accelerations->begin(),
                                      got.constraints->accelerations->end(),
                                      want.constraints->accelerations->begin(),
                                      want.constraints->accelerations->end());
    }
}

// A pull-source over a fixed list of batches, suitable as a batch_source. Each
// call yields the next batch, then boost::none. The batches are held by shared
// ownership so callers may inspect them after the stream completes.
std::function<boost::optional<std::vector<Arm::trajectory_point>>()> batch_pump(
    std::shared_ptr<std::vector<std::vector<Arm::trajectory_point>>> batches) {
    auto index = std::make_shared<std::size_t>(0);
    return [batches = std::move(batches),
            index]() -> boost::optional<std::vector<Arm::trajectory_point>> {
        if (*index >= batches->size()) {
            return boost::none;
        }
        return (*batches)[(*index)++];
    };
}

using Batches = std::vector<std::vector<Arm::trajectory_point>>;

using RawRequest = ::viam::component::arm::v1::MoveThroughJointPositionsStreamedRequest;
using RawResponse = ::viam::component::arm::v1::MoveThroughJointPositionsStreamedResponse;

// Builds the top-level Init request. The resource name lives at the top level
// of the request, not inside the Init payload.
RawRequest make_init(const std::string& name) {
    RawRequest request;
    request.set_name(name);
    request.mutable_init();
    return request;
}

// Drives the bidi RPC with a raw service stub, bypassing ArmClient so we can
// send sequences a typed client would never emit. Sends each request in order,
// half-closes, drains responses, and returns the terminal status.
//
// Responses are drained on a separate thread while the requests are written.
// This is not just tidiness: the server may write an update as soon as it
// accepts a valid batch, and if we wrote every request before reading any
// response, a server-side Write and our next Write could deadlock -- each side
// blocked in Write waiting for the other to read. Interleaving reader and
// writer mirrors what the real ArmClient does and keeps the wire draining.
grpc::Status drive_raw_stream(const std::shared_ptr<grpc::Channel>& channel,
                              const std::vector<RawRequest>& requests) {
    auto stub = ::viam::component::arm::v1::ArmService::NewStub(channel);
    grpc::ClientContext ctx;
    auto stream = stub->MoveThroughJointPositionsStreamed(&ctx);

    std::thread reader([&] {
        RawResponse response;
        while (stream->Read(&response)) {
        }
    });

    for (const auto& request : requests) {
        // A write may fail once the server has already errored and torn down;
        // that is expected for the malformed-input cases, so ignore it.
        if (!stream->Write(request)) {
            break;
        }
    }
    stream->WritesDone();

    reader.join();
    return stream->Finish();
}

}  // namespace

BOOST_AUTO_TEST_SUITE(test_arm)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockArm arm("mock_arm");
    auto api = arm.api();
    auto static_api = API::get<Arm>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "arm");
}

BOOST_AUTO_TEST_CASE(test_positions) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        pose p{{2, 3, 4}, {5, 6, 7}, 8};
        client.move_to_position(p);
        BOOST_CHECK_EQUAL(mock->current_location, p);
        BOOST_CHECK_EQUAL(mock->get_end_position({}), p);
    });
}

BOOST_AUTO_TEST_CASE(joint_positions) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        std::vector<double> positions{{1.0, 2.0, 3.0, 4.0}};
        client.move_to_joint_positions(positions, {});
        BOOST_CHECK_EQUAL(client.get_joint_positions(), positions);
        BOOST_CHECK_EQUAL(mock->joint_positions, positions);
    });
}

BOOST_AUTO_TEST_CASE(thru_joint_positions) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        std::vector<std::vector<double>> positions{{1.0, 2.0}, {3.0}};
        client.move_through_joint_positions(
            positions, {Arm::MoveLimit(1.0), Arm::MoveLimit(2.0)}, {});
        BOOST_CHECK_EQUAL(mock->move_thru_positions, positions);

        BOOST_REQUIRE(mock->move_opts.max_vel_degs_per_sec);
        BOOST_REQUIRE(mock->move_opts.max_acc_degs_per_sec2);
        boost::apply_visitor(CheckScalar(1.0), *mock->move_opts.max_vel_degs_per_sec);
        boost::apply_visitor(CheckScalar(2.0), *mock->move_opts.max_acc_degs_per_sec2);
        BOOST_CHECK_GT(mock->viam_client_metadata.size(), 0);
    });
}

BOOST_AUTO_TEST_CASE(thru_joint_positions_both_vector) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        std::vector<std::vector<double>> positions{{1.0, 2.0}, {3.0}};
        std::vector<double> vel_joints{10.0, 20.0, 30.0};
        std::vector<double> acc_joints{100.0, 200.0, 300.0};
        client.move_through_joint_positions(
            positions, {Arm::MoveLimit(vel_joints), Arm::MoveLimit(acc_joints)}, {});
        BOOST_CHECK_EQUAL(mock->move_thru_positions, positions);

        BOOST_REQUIRE(mock->move_opts.max_vel_degs_per_sec);
        BOOST_REQUIRE(mock->move_opts.max_acc_degs_per_sec2);
        boost::apply_visitor(CheckVector(vel_joints), *mock->move_opts.max_vel_degs_per_sec);
        boost::apply_visitor(CheckVector(acc_joints), *mock->move_opts.max_acc_degs_per_sec2);
    });
}

BOOST_AUTO_TEST_CASE(thru_joint_positions_acc_scalar_vel_vector) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        std::vector<std::vector<double>> positions{{1.0, 2.0}, {3.0}};
        std::vector<double> vel_joints{10.0, 20.0, 30.0};
        double acc_scalar = 5.0;
        client.move_through_joint_positions(
            positions, {Arm::MoveLimit(vel_joints), Arm::MoveLimit(acc_scalar)}, {});
        BOOST_CHECK_EQUAL(mock->move_thru_positions, positions);

        BOOST_REQUIRE(mock->move_opts.max_vel_degs_per_sec);
        BOOST_REQUIRE(mock->move_opts.max_acc_degs_per_sec2);
        boost::apply_visitor(CheckVector(vel_joints), *mock->move_opts.max_vel_degs_per_sec);
        boost::apply_visitor(CheckScalar(acc_scalar), *mock->move_opts.max_acc_degs_per_sec2);
    });
}

BOOST_AUTO_TEST_CASE(thru_joint_positions_vel_scalar_acc_vector) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        std::vector<std::vector<double>> positions{{1.0, 2.0}, {3.0}};
        double vel_scalar = 7.0;
        std::vector<double> acc_joints{100.0, 200.0, 300.0};
        client.move_through_joint_positions(
            positions, {Arm::MoveLimit(vel_scalar), Arm::MoveLimit(acc_joints)}, {});
        BOOST_CHECK_EQUAL(mock->move_thru_positions, positions);

        BOOST_REQUIRE(mock->move_opts.max_vel_degs_per_sec);
        BOOST_REQUIRE(mock->move_opts.max_acc_degs_per_sec2);
        boost::apply_visitor(CheckScalar(vel_scalar), *mock->move_opts.max_vel_degs_per_sec);
        boost::apply_visitor(CheckVector(acc_joints), *mock->move_opts.max_acc_degs_per_sec2);
    });
}

BOOST_AUTO_TEST_CASE(thru_joint_positions_max_tcp_speed) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        std::vector<std::vector<double>> positions{{1.0, 2.0}, {3.0}};
        Arm::MoveOptions opts;
        opts.max_vel_degs_per_sec = Arm::MoveLimit(1.0);
        opts.max_acc_degs_per_sec2 = Arm::MoveLimit(2.0);
        opts.max_tcp_speed = 42.0;
        client.move_through_joint_positions(positions, opts, {});
        BOOST_CHECK_EQUAL(mock->move_thru_positions, positions);

        BOOST_REQUIRE(mock->move_opts.max_tcp_speed);
        BOOST_CHECK_EQUAL(*mock->move_opts.max_tcp_speed, 42.0);
    });
}

BOOST_AUTO_TEST_CASE(test_stop) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        mock->peek_stop_called = false;
        client.stop();
        BOOST_CHECK(mock->peek_stop_called);
    });
}

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [](Arm& client) {
        const auto& geometries = client.get_geometries();
        BOOST_CHECK_EQUAL(geometries, fake_geometries());
    });
}

BOOST_AUTO_TEST_CASE(test_get_kinematics) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [](Arm& client) {
        const auto& kinematics = client.get_kinematics();
        BOOST_CHECK_EQUAL(kinematics, fake_kinematics());
    });
}

BOOST_AUTO_TEST_CASE(test_is_moving) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [](Arm& client) { BOOST_CHECK(!client.is_moving()); });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [](Arm& client) {
        ProtoStruct expected = fake_map();

        ProtoStruct command = fake_map();
        ProtoStruct result_map = client.do_command(command);

        BOOST_CHECK(result_map.at("test") == expected.at("test"));
    });
}

BOOST_AUTO_TEST_CASE(test_get_status) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [](Arm& client) {
        const ProtoStruct status = client.get_status();
        const ProtoStruct expected = fake_status();
        BOOST_CHECK(status.at("is_moving") == expected.at("is_moving"));
    });
}

BOOST_AUTO_TEST_CASE(test_get_3d_models) {
    std::shared_ptr<MockArm> mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [](Arm& client) {
        const auto& models = client.get_3d_models();
        const auto& expected = fake_3d_models();

        // Compare keysets directly
        std::set<std::string> model_keys;
        std::set<std::string> expected_keys;
        for (const auto& [key, _] : models) {
            model_keys.insert(key);
        }
        for (const auto& [key, _] : expected) {
            expected_keys.insert(key);
        }
        BOOST_CHECK_EQUAL_COLLECTIONS(
            model_keys.begin(), model_keys.end(), expected_keys.begin(), expected_keys.end());
    });
}

BOOST_AUTO_TEST_CASE(streamed_trajectory_point_proto_roundtrip) {
    // Full point: an odd microsecond count to catch precision loss, plus both
    // velocities and accelerations.
    const auto full = make_point(1234567,
                                 {1.0, 2.0, 3.0},
                                 std::vector<double>{4.0, 5.0, 6.0},
                                 std::vector<double>{7.0, 8.0, 9.0});
    check_points_equal(from_proto(to_proto(full)), full);

    // Constraints with velocities but no accelerations.
    const auto no_accel = make_point(10, {1.0, 2.0}, std::vector<double>{4.0, 5.0});
    check_points_equal(from_proto(to_proto(no_accel)), no_accel);

    // No constraints at all.
    const auto bare = make_point(0, {1.0, 2.0});
    check_points_equal(from_proto(to_proto(bare)), bare);
}

BOOST_AUTO_TEST_CASE(streamed_happy_path) {
    auto mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        auto batches = std::make_shared<Batches>(Batches{
            {make_point(0, {1.0}), make_point(10, {2.0})},
            {make_point(20, {3.0}), make_point(30, {4.0})},
        });

        int client_acks = 0;
        const auto outcome =
            client.move_through_joint_positions_streamed(batch_pump(batches),
                                                         [&](Arm::trajectory_update) {
                                                             ++client_acks;
                                                             return true;
                                                         },
                                                         {});

        BOOST_CHECK(outcome == Arm::stream_outcome::k_completed);
        BOOST_CHECK_EQUAL(client_acks, 2);
        BOOST_CHECK_EQUAL(mock->peek_streamed_ack_count, 2);
        BOOST_REQUIRE_EQUAL(mock->peek_streamed_batches.size(), 2U);
        BOOST_REQUIRE_EQUAL(mock->peek_streamed_batches[0].size(), 2U);
        BOOST_REQUIRE_EQUAL(mock->peek_streamed_batches[1].size(), 2U);
        check_points_equal(mock->peek_streamed_batches[0][0], (*batches)[0][0]);
        check_points_equal(mock->peek_streamed_batches[1][1], (*batches)[1][1]);
    });
}

BOOST_AUTO_TEST_CASE(streamed_empty_batches_filtered) {
    auto mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        // The empty middle batch is dropped on the wire and never reaches the
        // impl; the surrounding points still form a valid monotonic stream.
        auto batches = std::make_shared<Batches>(Batches{
            {make_point(0, {1.0})},
            {},
            {make_point(10, {2.0})},
        });

        int client_acks = 0;
        client.move_through_joint_positions_streamed(batch_pump(batches),
                                                     [&](Arm::trajectory_update) {
                                                         ++client_acks;
                                                         return true;
                                                     },
                                                     {});

        BOOST_CHECK_EQUAL(mock->peek_streamed_batches.size(), 2U);
        BOOST_CHECK_EQUAL(mock->peek_streamed_ack_count, 2);
        BOOST_CHECK_EQUAL(client_acks, 2);
    });
}

BOOST_AUTO_TEST_CASE(streamed_impl_runtime_error_propagates) {
    auto mock = MockArm::get_mock_arm();
    mock->streamed_fault = MockArm::StreamFault::runtime_error;
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        auto batches = std::make_shared<Batches>(Batches{{make_point(0, {1.0})}});
        BOOST_CHECK_THROW(client.move_through_joint_positions_streamed(
                              batch_pump(batches), [](Arm::trajectory_update) { return true; }, {}),
                          GRPCException);
    });
}

BOOST_AUTO_TEST_CASE(streamed_impl_grpc_status_propagates) {
    auto mock = MockArm::get_mock_arm();
    mock->streamed_fault = MockArm::StreamFault::grpc_status;
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        auto batches = std::make_shared<Batches>(Batches{{make_point(0, {1.0})}});
        try {
            client.move_through_joint_positions_streamed(
                batch_pump(batches), [](Arm::trajectory_update) { return true; }, {});
            BOOST_FAIL("expected a GRPCException carrying the impl's status");
        } catch (const GRPCException& e) {
            BOOST_CHECK(e.status()->error_code() == grpc::StatusCode::FAILED_PRECONDITION);
        }
    });
}

BOOST_AUTO_TEST_CASE(streamed_update_handler_halt) {
    auto mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        auto batches = std::make_shared<Batches>(Batches{
            {make_point(0, {1.0})},
            {make_point(10, {2.0})},
        });
        // Halting on the first update is a deliberate stop, not a fault: the
        // call returns the halted outcome rather than throwing.
        const auto outcome = client.move_through_joint_positions_streamed(
            batch_pump(batches), [](Arm::trajectory_update) { return false; }, {});
        BOOST_CHECK(outcome == Arm::stream_outcome::k_halted_by_update_handler);
    });
}

BOOST_AUTO_TEST_CASE(streamed_update_handler_throw_propagates) {
    auto mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        auto batches = std::make_shared<Batches>(Batches{{make_point(0, {1.0})}});
        // A throwing handler propagates its exception as-is, not wrapped as a
        // GRPCException and not swallowed into an outcome.
        try {
            client.move_through_joint_positions_streamed(
                batch_pump(batches),
                [](Arm::trajectory_update) -> bool { throw std::runtime_error("handler blew up"); },
                {});
            BOOST_FAIL("expected the handler's exception to propagate");
        } catch (const GRPCException&) {
            BOOST_FAIL("handler exception should propagate as-is, not as a GRPCException");
        } catch (const std::runtime_error& e) {
            BOOST_CHECK_EQUAL(std::string(e.what()), "handler blew up");
        }
    });
}

BOOST_AUTO_TEST_CASE(streamed_client_rejects_invalid_trajectories) {
    auto mock = MockArm::get_mock_arm();
    client_to_mock_pipeline<Arm>(mock, [&](Arm& client) {
        // Each malformed trajectory must be rejected locally, as a plain
        // Exception, before any of it reaches the server as a GRPCException.
        const auto expect_local_reject = [&](Batches bad) {
            auto batches = std::make_shared<Batches>(std::move(bad));
            try {
                client.move_through_joint_positions_streamed(
                    batch_pump(batches), [](Arm::trajectory_update) { return true; }, {});
                BOOST_FAIL("expected a local validation exception");
            } catch (const GRPCException&) {
                BOOST_FAIL("expected a local validation Exception, not a GRPCException");
            } catch (const Exception& e) {
                BOOST_CHECK(std::string(e.what()).find("move_through_joint_positions_streamed") !=
                            std::string::npos);
            }
        };

        expect_local_reject({{make_point(5, {1.0})}});
        expect_local_reject({{make_point(0, {1.0}), make_point(0, {2.0})}});
        expect_local_reject({{make_point(0, {})}});
        expect_local_reject({{make_point(0, {1.0}, std::vector<double>{1.0, 2.0})}});
        // Cross-batch: the second batch rewinds time relative to the first
        // batch's last point. Exercises the validator's state carried across
        // batch boundaries, not just within a single batch.
        expect_local_reject(
            {{make_point(0, {1.0}), make_point(10, {2.0})}, {make_point(10, {3.0})}});
    });
}

BOOST_AUTO_TEST_CASE(streamed_server_rejects_stream_without_init) {
    auto mock = MockArm::get_mock_arm();
    channel_to_mock_pipeline(mock, [&](const std::shared_ptr<grpc::Channel>& channel) {
        // Half-close immediately, sending nothing.
        BOOST_CHECK(drive_raw_stream(channel, {}).error_code() ==
                    grpc::StatusCode::INVALID_ARGUMENT);
    });
}

BOOST_AUTO_TEST_CASE(streamed_server_requires_init_first) {
    auto mock = MockArm::get_mock_arm();
    channel_to_mock_pipeline(mock, [&](const std::shared_ptr<grpc::Channel>& channel) {
        // A Batch as the very first message is a protocol violation.
        RawRequest batch_first;
        *batch_first.mutable_batch()->add_points() = to_proto(make_point(0, {1.0}));
        BOOST_CHECK(drive_raw_stream(channel, {batch_first}).error_code() ==
                    grpc::StatusCode::INVALID_ARGUMENT);
    });
}

BOOST_AUTO_TEST_CASE(streamed_server_rejects_second_init) {
    auto mock = MockArm::get_mock_arm();
    channel_to_mock_pipeline(mock, [&](const std::shared_ptr<grpc::Channel>& channel) {
        BOOST_CHECK(drive_raw_stream(channel, {make_init(mock->name()), make_init(mock->name())})
                        .error_code() == grpc::StatusCode::INVALID_ARGUMENT);
    });
}

BOOST_AUTO_TEST_CASE(streamed_server_rejects_non_batch_payload) {
    auto mock = MockArm::get_mock_arm();
    channel_to_mock_pipeline(mock, [&](const std::shared_ptr<grpc::Channel>& channel) {
        // A request with neither oneof arm set, after a valid Init.
        RawRequest empty_oneof;
        BOOST_CHECK(
            drive_raw_stream(channel, {make_init(mock->name()), empty_oneof}).error_code() ==
            grpc::StatusCode::INVALID_ARGUMENT);
    });
}

BOOST_AUTO_TEST_CASE(streamed_server_validates_trajectory) {
    auto mock = MockArm::get_mock_arm();
    channel_to_mock_pipeline(mock, [&](const std::shared_ptr<grpc::Channel>& channel) {
        // The server enforces the trajectory invariants itself, independent of
        // any client-side check, because callers need not reach it through
        // ArmClient. First point with a nonzero time:
        RawRequest bad_time;
        *bad_time.mutable_batch()->add_points() = to_proto(make_point(5, {1.0}));
        BOOST_CHECK(drive_raw_stream(channel, {make_init(mock->name()), bad_time}).error_code() ==
                    grpc::StatusCode::INVALID_ARGUMENT);

        // Velocities that do not match the position count:
        RawRequest bad_dims;
        *bad_dims.mutable_batch()->add_points() =
            to_proto(make_point(0, {1.0}, std::vector<double>{1.0, 2.0}));
        BOOST_CHECK(drive_raw_stream(channel, {make_init(mock->name()), bad_dims}).error_code() ==
                    grpc::StatusCode::INVALID_ARGUMENT);

        // None of the above malformed single points ever reached the impl.
        BOOST_CHECK(mock->peek_streamed_batches.empty());

        // Cross-batch monotonicity: a valid first batch followed by a second
        // batch that rewinds time. The server's validator carries state across
        // batches, so this is rejected even though each batch is internally
        // fine. The valid first batch does reach the impl before the second is
        // rejected, so this case comes after the empty check above.
        RawRequest batch_a;
        *batch_a.mutable_batch()->add_points() = to_proto(make_point(0, {1.0}));
        *batch_a.mutable_batch()->add_points() = to_proto(make_point(10, {2.0}));
        RawRequest batch_b;
        *batch_b.mutable_batch()->add_points() = to_proto(make_point(10, {3.0}));
        BOOST_CHECK(
            drive_raw_stream(channel, {make_init(mock->name()), batch_a, batch_b}).error_code() ==
            grpc::StatusCode::INVALID_ARGUMENT);
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
