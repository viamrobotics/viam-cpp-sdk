#pragma once

#include <cstdint>

#include <viam/sdk/components/arm.hpp>

namespace viam {
namespace sdktests {
namespace arm {

sdk::KinematicsData fake_kinematics();
std::map<std::string, sdk::mesh> fake_3d_models();

class MockArm : public sdk::Arm {
   public:
    MockArm(std::string name) : Arm(std::move(name)) {}

    static std::shared_ptr<MockArm> get_mock_arm();

    sdk::pose get_end_position(const sdk::ProtoStruct&) override;
    void move_to_position(const sdk::pose& pose, const sdk::ProtoStruct&) override;
    std::vector<double> get_joint_positions(const sdk::ProtoStruct&) override;
    void move_to_joint_positions(const std::vector<double>& positions,
                                 const sdk::ProtoStruct&) override;

    void move_through_joint_positions(const std::vector<std::vector<double>>& positions,
                                      const Arm::MoveOptions& opts,
                                      const sdk::ProtoStruct&) override;
    void move_through_joint_positions_streamed(
        std::function<boost::optional<std::vector<Arm::TrajectoryPoint>>()> batch_source,
        std::function<bool(Arm::Response)> response_sink,
        const sdk::ProtoStruct&) override;

    void stop(const sdk::ProtoStruct&) override;
    bool is_moving() override;
    sdk::ProtoStruct get_status() override;
    sdk::ProtoStruct do_command(const sdk::ProtoStruct& command) override;
    sdk::KinematicsData get_kinematics(const sdk::ProtoStruct&) override;
    std::vector<sdk::GeometryConfig> get_geometries(const sdk::ProtoStruct&) override;
    std::map<std::string, sdk::mesh> get_3d_models(const sdk::ProtoStruct&) override;
    sdk::pose current_location;
    std::vector<double> joint_positions;
    std::vector<std::vector<double>> move_thru_positions;
    sdk::Arm::MoveOptions move_opts;
    bool peek_stop_called;
    sdk::ProtoStruct peek_command;
    std::vector<std::string> viam_client_metadata;

    // Streaming trajectory hooks. move_through_joint_positions_streamed records
    // each batch it receives into peek_streamed_batches and counts the acks it
    // emits in peek_streamed_ack_count. If streamed_fault is set, it throws
    // after draining to exercise terminal-error propagation: runtime_error maps
    // to a std::exception (INTERNAL at the wire), grpc_status to a thrown
    // grpc::Status carried through verbatim.
    enum class StreamFault : std::uint8_t { none, runtime_error, grpc_status };
    StreamFault streamed_fault = StreamFault::none;
    std::vector<std::vector<sdk::Arm::TrajectoryPoint>> peek_streamed_batches;
    int peek_streamed_ack_count = 0;
};

}  // namespace arm
}  // namespace sdktests
}  // namespace viam
