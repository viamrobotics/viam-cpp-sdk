#include <boost/variant/apply_visitor.hpp>
#include <viam/sdk/components/private/arm_client.hpp>

#include <exception>
#include <thread>
#include <utility>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>

#include <viam/api/component/arm/v1/arm.grpc.pb.h>
#include <viam/api/component/arm/v1/arm.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/kinematics.hpp>
#include <viam/sdk/components/private/arm_trajectory_validation.hpp>

namespace viam {
namespace sdk {
namespace impl {

using sdk::from_proto;
using sdk::to_proto;

ArmClient::ArmClient(std::string name, const ViamChannel& channel)
    : Arm(std::move(name)),
      stub_(viam::component::arm::v1::ArmService::NewStub(channel.channel())),
      channel_(&channel) {}

pose ArmClient::get_end_position(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetEndPosition)
        .with(extra)
        .invoke([&](auto& response) { return from_proto(response.pose()); });
}

void ArmClient::move_to_position(const pose& pose, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::MoveToPosition)
        .with(extra, [&](auto& request) { *request.mutable_to() = to_proto(pose); })
        .invoke();
}

std::vector<double> ArmClient::get_joint_positions(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetJointPositions)
        .with(extra)
        .invoke([](auto& response) {
            return std::vector<double>(response.positions().values().begin(),
                                       response.positions().values().end());
        });
}

void ArmClient::move_to_joint_positions(const std::vector<double>& positions,
                                        const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::MoveToJointPositions)
        .with(extra,
              [&](auto& request) {
                  *(request.mutable_positions()->mutable_values()) = {positions.begin(),
                                                                      positions.end()};
              })
        .invoke();
}

void ArmClient::move_through_joint_positions(const std::vector<std::vector<double>>& positions,
                                             const Arm::MoveOptions& options,
                                             const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::MoveThroughJointPositions)
        .with(extra,
              [&](viam::component::arm::v1::MoveThroughJointPositionsRequest& request) {
                  if (options.max_vel_degs_per_sec) {
                      struct Visitor {
                          viam::component::arm::v1::MoveOptions* opts;
                          void operator()(double s) const {
                              opts->set_max_vel_degs_per_sec(s);
                          }
                          void operator()(const std::vector<double>& v) const {
                              for (auto d : v) {
                                  opts->add_max_vel_degs_per_sec_joints(d);
                              }
                          }
                      };
                      boost::apply_visitor(Visitor{request.mutable_options()},
                                           *options.max_vel_degs_per_sec);
                  }

                  if (options.max_acc_degs_per_sec2) {
                      struct Visitor {
                          viam::component::arm::v1::MoveOptions* opts;
                          void operator()(double s) const {
                              opts->set_max_acc_degs_per_sec2(s);
                          }
                          void operator()(const std::vector<double>& v) const {
                              for (auto d : v) {
                                  opts->add_max_acc_degs_per_sec2_joints(d);
                              }
                          }
                      };
                      boost::apply_visitor(Visitor{request.mutable_options()},
                                           *options.max_acc_degs_per_sec2);
                  }

                  if (options.max_tcp_speed) {
                      request.mutable_options()->set_max_tcp_speed(*options.max_tcp_speed);
                  }

                  for (const auto& pos : positions) {
                      viam::component::arm::v1::JointPositions jpos;
                      jpos.mutable_values()->Add(pos.begin(), pos.end());
                      request.mutable_positions()->Add(std::move(jpos));
                  }
              })
        .invoke();
}

Arm::stream_outcome ArmClient::move_through_joint_positions_streamed(
    const std::function<boost::optional<std::vector<Arm::trajectory_point>>()>& batch_source,
    const std::function<bool(Arm::trajectory_update)>& update_handler,
    const ProtoStruct& extra) {
    // Use the SDK's ClientContext wrapper rather than a raw grpc::ClientContext.
    // The wrapper's channel-aware constructor attaches the authorization Bearer
    // token (required for authenticated Viam cloud connections), the
    // viam_client version metadata, the macOS authority workaround
    // (RSDK-5194), and the OpenTelemetry trace context. ClientHelper does this
    // automatically for non-bidi RPCs; hand-rolled streaming has to do it here.
    ClientContext ctx(*channel_);
    auto stream = stub_->MoveThroughJointPositionsStreamed(ctx);

    // Send Init on the caller's thread before any other thread is involved.
    ::viam::component::arm::v1::MoveThroughJointPositionsStreamedRequest init_msg;
    init_msg.set_name(this->name());
    auto* init_payload = init_msg.mutable_init();
    *init_payload->mutable_extra() = to_proto(extra);
    if (!stream->Write(init_msg)) {
        const auto status = stream->Finish();
        throw GRPCException(&status);
    }

    // One reader thread: read updates, invoke update_handler, exit on close.
    // If update_handler returns false the caller is electing to halt; record it
    // so teardown reports k_halted_by_update_handler rather than the CANCELLED
    // status our own TryCancel will produce.
    bool update_handler_halted = false;
    std::exception_ptr reader_exception;
    std::thread reader([&] {
        try {
            ::viam::component::arm::v1::MoveThroughJointPositionsStreamedResponse pb;
            while (stream->Read(&pb)) {
                if (!update_handler(Arm::trajectory_update{})) {
                    update_handler_halted = true;
                    ctx.try_cancel();
                    return;
                }
            }
        } catch (...) {
            reader_exception = std::current_exception();
            ctx.try_cancel();
        }
    });

    // Writer loop runs on the caller's thread.
    std::exception_ptr writer_exception;
    try {
        // Fail fast on a malformed trajectory rather than shipping it. The
        // server revalidates, since callers need not reach it through this
        // client, but catching it here spares a wasted round trip. The
        // validator persists across batches to enforce the stream-wide
        // monotonic-time contract.
        TrajectoryStreamValidator validator;
        while (auto batch = batch_source()) {
            // Avoid emitting empty batches on the wire.
            if (batch->empty()) {
                continue;
            }
            ::viam::component::arm::v1::MoveThroughJointPositionsStreamedRequest msg;
            auto* trajectory_batch = msg.mutable_batch();
            for (const auto& point : *batch) {
                if (const auto err = validator.check(point)) {
                    throw Exception(ErrorCondition::k_general,
                                    "move_through_joint_positions_streamed: " + *err);
                }
                *trajectory_batch->add_points() = to_proto(point);
            }
            if (!stream->Write(msg)) {
                break;
            }
        }
        stream->WritesDone();
    } catch (...) {
        writer_exception = std::current_exception();
        ctx.try_cancel();
    }

    reader.join();

    // Reap the call on every exit path, even the ones that TryCancel'd and are
    // about to rethrow. Finish() completes the cancelled RPC and releases its
    // resources; skipping it (as the error paths previously did) leaks an
    // in-flight call per errored stream. It is safe to call now: the reader has
    // been joined, so no other thread touches the stream.
    const auto status = stream->Finish();

    // Prefer a callback exception over Finish()'s gRPC status: after a
    // TryCancel that status is a generic CANCELLED that hides the real cause,
    // whereas the stashed exception carries it.
    if (writer_exception) {
        std::rethrow_exception(writer_exception);
    }
    if (reader_exception) {
        std::rethrow_exception(reader_exception);
    }

    // A deliberate caller-driven stop is an outcome, not a fault: report it
    // rather than throwing the CANCELLED status our own TryCancel produced.
    // This is checked after the exception rethrows above by design: if the
    // caller both halted and something faulted, the fault is the more urgent
    // signal and wins.
    if (update_handler_halted) {
        return Arm::stream_outcome::k_halted_by_update_handler;
    }

    if (!status.ok()) {
        throw GRPCException(&status);
    }

    return Arm::stream_outcome::k_completed;
}

bool ArmClient::is_moving() {
    return make_client_helper(this, *stub_, &StubType::IsMoving).invoke([](auto& response) {
        return response.is_moving();
    });
}

ProtoStruct ArmClient::get_status() {
    return make_client_helper(this, *stub_, &StubType::GetStatus).invoke([](auto& response) {
        return from_proto(response.result());
    });
}

void ArmClient::stop(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Stop).with(extra).invoke();
}

ProtoStruct ArmClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
}

::viam::sdk::KinematicsData ArmClient::get_kinematics(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetKinematics)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

std::map<std::string, mesh> ArmClient::get_3d_models(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Get3DModels)
        .with(extra)
        .invoke([](auto& response) {
            std::map<std::string, mesh> models;
            for (const auto& entry : response.models()) {
                models.emplace(entry.first, from_proto(entry.second));
            }
            return models;
        });
}

std::vector<GeometryConfig> ArmClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
