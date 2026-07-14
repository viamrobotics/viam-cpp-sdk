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
    // TODO(RSDK-14164): this hand-rolls the BiDi stream because the SDK has no
    // BiDi client/server helper yet (RSDK-14164 tracks adding one). We use the
    // SDK's `ClientContext` wrapper rather than a raw `grpc::ClientContext` so
    // the call carries what `ClientHelper` would attach for a unary RPC: the
    // authorization bearer token (needed for authenticated cloud connections),
    // the `viam_client` version metadata, the macOS authority workaround
    // (RSDK-5194), and the OpenTelemetry trace context.
    ClientContext ctx(*channel_);
    auto stream = stub_->MoveThroughJointPositionsStreamed(ctx);

    // Send `Init` on the caller's thread. If it fails there is no reader yet and
    // nothing to tear down, so reap and report right here.
    ::viam::component::arm::v1::MoveThroughJointPositionsStreamedRequest init_msg;
    init_msg.set_name(this->name());
    *init_msg.mutable_init()->mutable_extra() = to_proto(extra);
    if (!stream->Write(init_msg)) {
        const ::grpc::Status status = stream->Finish();
        throw GRPCException(&status);
    }

    // `Init` is on the wire, so from here we always have a reader to join and a
    // call to reap. `Finish()` reaps the RPC (an unfinished call leaks), and its
    // status is how a server-side fault reaches us on the happy path: the server
    // aborts, our `Read` and `Write` stop returning true, and `Finish()` reports
    // why. We need that status for the decision after the block, and a destructor
    // runs too late to hand back a value, so the guard's only job is to run
    // `Finish()` on every way out of the block and stash its status where we can
    // read it. The reader is joined inside the block, so `Finish()` runs after it.
    ::grpc::Status finish_status;
    struct finish_guard {
        decltype(stream.get()) stream;
        ::grpc::Status& status;
        ~finish_guard() {
            status = stream->Finish();
        }
    };

    bool update_handler_halted = false;
    std::exception_ptr writer_exception;
    std::exception_ptr reader_exception;

    {
        const finish_guard guard{stream.get(), finish_status};

        // Reader thread: read each response, turn it into a `trajectory_update`,
        // and pass it to `update_handler`. A false return is the caller asking to
        // stop, so we remember it and report `k_halted_by_update_handler` rather
        // than the `CANCELLED` status our own `try_cancel` produces.
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

        // Writer loop, on the caller's thread. Validating the trajectory is the
        // server's job (a caller need not reach the server through this client at
        // all), so we send whatever we are handed.
        try {
            while (auto batch = batch_source()) {
                ::viam::component::arm::v1::MoveThroughJointPositionsStreamedRequest msg;
                auto* trajectory_batch = msg.mutable_batch();
                for (const auto& point : *batch) {
                    *trajectory_batch->add_points() = to_proto(point);
                }
                if (!stream->Write(msg)) {
                    // A false `Write` means the server closed the stream, which
                    // it does when it faults or cancels. Nothing to raise here:
                    // stop writing, and `finish_status` will carry the server's
                    // terminal status.
                    break;
                }
            }
            stream->WritesDone();
        } catch (...) {
            writer_exception = std::current_exception();
            ctx.try_cancel();
        }

        reader.join();
    }

    // A stashed callback exception beats `finish_status`, which after a
    // `try_cancel` is a bare `CANCELLED` that hides the real cause. If both
    // threads stashed one, take the writer's: it runs on the caller's thread and
    // is the likely origin (say `batch_source` threw), while the reader's is
    // usually just fallout from the same teardown.
    if (writer_exception) {
        std::rethrow_exception(writer_exception);
    }
    if (reader_exception) {
        std::rethrow_exception(reader_exception);
    }

    // A caller-driven stop is an outcome, not a fault. Checked after the
    // rethrows: if the caller stopped and something also faulted, the fault wins.
    if (update_handler_halted) {
        return Arm::stream_outcome::k_halted_by_update_handler;
    }

    if (!finish_status.ok()) {
        throw GRPCException(&finish_status);
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
