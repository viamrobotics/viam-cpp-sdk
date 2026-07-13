#include <viam/sdk/components/private/arm_client.hpp>

#include <exception>
#include <thread>
#include <utility>

#include <boost/variant/apply_visitor.hpp>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>

#include <viam/api/component/arm/v1/arm.grpc.pb.h>
#include <viam/api/component/arm/v1/arm.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/kinematics.hpp>

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
    // TODO(RSDK-14164): this method hand-rolls the bidi stream because the SDK
    // has no bidi client/server helper yet; RSDK-14164 tracks adding one, at
    // which point this drops down onto that. Until then we drive grpc++
    // directly, and use the SDK's ClientContext wrapper rather than a raw
    // grpc::ClientContext so the call carries what ClientHelper attaches
    // automatically for non-bidi RPCs: the authorization Bearer token (required
    // for authenticated Viam cloud connections), the viam_client version
    // metadata, the macOS authority workaround (RSDK-5194), and the
    // OpenTelemetry trace context.
    ClientContext ctx(*channel_);
    auto stream = stub_->MoveThroughJointPositionsStreamed(ctx);

    // Reap the RPC exactly once, on every exit path. Finish() completes the call
    // -- including one we cancelled via try_cancel -- and releases its
    // resources; an early return that skipped it would leak an in-flight call.
    // The reader must be joined before Finish() runs: we call reap() explicitly
    // once it has, and the destructor is only a backstop for the paths that
    // throw before a reader is even running.
    struct stream_reaper {
        decltype(stream.get()) stream;
        ::grpc::Status status;
        bool reaped = false;
        const ::grpc::Status& reap() {
            if (!reaped) {
                reaped = true;
                status = stream->Finish();
            }
            return status;
        }
        ~stream_reaper() {
            reap();
        }
    } reaper{stream.get()};

    // Send Init on the caller's thread before any other thread is involved.
    ::viam::component::arm::v1::MoveThroughJointPositionsStreamedRequest init_msg;
    init_msg.set_name(this->name());
    auto* init_payload = init_msg.mutable_init();
    *init_payload->mutable_extra() = to_proto(extra);
    if (!stream->Write(init_msg)) {
        throw GRPCException(&reaper.reap());
    }

    // One reader thread: read each response, convert it to a trajectory_update,
    // and hand it to update_handler. A false return means the caller is electing
    // to halt; record it so teardown reports k_halted_by_update_handler rather
    // than the CANCELLED status our own try_cancel will produce.
    bool update_handler_halted = false;
    std::exception_ptr reader_exception;
    std::thread reader([&] {
        try {
            ::viam::component::arm::v1::MoveThroughJointPositionsStreamedResponse pb;
            while (stream->Read(&pb)) {
                if (!update_handler(from_proto(pb))) {
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

    // Writer loop runs on the caller's thread. Validation is the server's job
    // (it cannot trust callers, which need not reach it through this client), so
    // we do not revalidate here.
    std::exception_ptr writer_exception;
    try {
        while (auto batch = batch_source()) {
            // Skip empty batches rather than putting a no-op message on the wire.
            if (batch->empty()) {
                continue;
            }
            ::viam::component::arm::v1::MoveThroughJointPositionsStreamedRequest msg;
            auto* trajectory_batch = msg.mutable_batch();
            for (const auto& point : *batch) {
                *trajectory_batch->add_points() = to_proto(point);
            }
            if (!stream->Write(msg)) {
                // A false Write means the server has closed the stream, which it
                // does when it faults or cancels. There is nothing to raise
                // here: we stop writing and fall through to WritesDone and the
                // join, after which reap() below surfaces the server's terminal
                // status as the call's result.
                break;
            }
        }
        stream->WritesDone();
    } catch (...) {
        writer_exception = std::current_exception();
        ctx.try_cancel();
    }

    reader.join();

    // Reap now that the reader has joined and no other thread touches the stream.
    const auto& status = reaper.reap();

    // Prefer a stashed callback exception over Finish()'s status: after a
    // try_cancel that status is a generic CANCELLED that hides the real cause,
    // whereas the exception carries it. When both threads stashed one the
    // writer's wins, because it runs on the caller's thread and reflects the
    // originating fault (e.g. batch_source threw), while the reader's is usually
    // just the downstream consequence of the same teardown.
    if (writer_exception) {
        std::rethrow_exception(writer_exception);
    }
    if (reader_exception) {
        std::rethrow_exception(reader_exception);
    }

    // A deliberate caller-driven stop is an outcome, not a fault: report it
    // rather than throwing the CANCELLED status our own try_cancel produced.
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
