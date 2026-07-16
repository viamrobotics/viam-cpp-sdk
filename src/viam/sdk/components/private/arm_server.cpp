#include <stdexcept>
#include <viam/sdk/components/private/arm_server.hpp>

#include <utility>
#include <vector>

#include <viam/sdk/common/kinematics.hpp>
#include <viam/sdk/common/private/service_helper.hpp>
#include <viam/sdk/components/private/arm_trajectory_validation.hpp>
#include <viam/sdk/rpc/private/grpc_context_observer.hpp>
#include <viam/sdk/tracing/private/span_guard.hpp>

namespace viam {
namespace sdk {
namespace impl {

ArmServer::ArmServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status ArmServer::GetEndPosition(
    ::grpc::ServerContext* context,
    const ::viam::component::arm::v1::GetEndPositionRequest* request,
    ::viam::component::arm::v1::GetEndPositionResponse* response) noexcept {
    return make_service_helper<Arm>(
        "ArmServer::GetEndPosition", this, context, request)([&](auto& helper, auto& arm) {
        const pose p = arm->get_end_position(helper.getExtra());
        *response->mutable_pose() = to_proto(p);
    });
}

::grpc::Status ArmServer::MoveToPosition(
    ::grpc::ServerContext* context,
    const ::viam::component::arm::v1::MoveToPositionRequest* request,
    ::viam::component::arm::v1::MoveToPositionResponse*) noexcept {
    return make_service_helper<Arm>(
        "ArmServer::MoveToPosition", this, context, request)([&](auto& helper, auto& arm) {
        arm->move_to_position(from_proto(request->to()), helper.getExtra());
    });
}

::grpc::Status ArmServer::GetJointPositions(
    ::grpc::ServerContext* context,
    const ::viam::component::arm::v1::GetJointPositionsRequest* request,
    ::viam::component::arm::v1::GetJointPositionsResponse* response) noexcept {
    return make_service_helper<Arm>(
        "ArmServer::GetJointPositions", this, context, request)([&](auto& helper, auto& arm) {
        const std::vector<double> positions = arm->get_joint_positions(helper.getExtra());
        *(response->mutable_positions()->mutable_values()) = {positions.begin(), positions.end()};
    });
}

::grpc::Status ArmServer::MoveToJointPositions(
    ::grpc::ServerContext* context,
    const ::viam::component::arm::v1::MoveToJointPositionsRequest* request,
    ::viam::component::arm::v1::MoveToJointPositionsResponse*) noexcept {
    return make_service_helper<Arm>(
        "ArmServer::MoveToJointPositions", this, context, request)([&](auto& helper, auto& arm) {
        arm->move_to_joint_positions(
            {request->positions().values().begin(), request->positions().values().end()},
            helper.getExtra());
    });
}

::grpc::Status ArmServer::MoveThroughJointPositions(
    ::grpc::ServerContext* context,
    const ::viam::component::arm::v1::MoveThroughJointPositionsRequest* request,
    ::viam::component::arm::v1::MoveThroughJointPositionsResponse*) noexcept {
    return make_service_helper<Arm>("ArmServer::MoveThroughJointPositions", this, context, request)(
        [&](auto& helper, auto& arm) {
            std::vector<std::vector<double>> positions;

            positions.reserve(request->positions_size());
            for (const auto& values : request->positions()) {
                positions.emplace_back(values.values().begin(), values.values().end());
            }

            Arm::MoveOptions opts;
            if (request->options().max_vel_degs_per_sec_joints_size() > 0) {
                opts.max_vel_degs_per_sec =
                    std::vector<double>(request->options().max_vel_degs_per_sec_joints().begin(),
                                        request->options().max_vel_degs_per_sec_joints().end());
            } else if (request->options().has_max_vel_degs_per_sec()) {
                opts.max_vel_degs_per_sec = request->options().max_vel_degs_per_sec();
            }

            if (request->options().max_acc_degs_per_sec2_joints_size() > 0) {
                opts.max_acc_degs_per_sec2 =
                    std::vector<double>(request->options().max_acc_degs_per_sec2_joints().begin(),
                                        request->options().max_acc_degs_per_sec2_joints().end());
            } else if (request->options().has_max_acc_degs_per_sec2()) {
                opts.max_acc_degs_per_sec2 = request->options().max_acc_degs_per_sec2();
            }

            if (request->options().has_max_tcp_speed()) {
                opts.max_tcp_speed = request->options().max_tcp_speed();
            }

            arm->move_through_joint_positions(positions, opts, helper.getExtra());
        });
}

::grpc::Status ArmServer::MoveThroughJointPositionsStreamed(
    ::grpc::ServerContext* context,
    ::grpc::ServerReaderWriter<
        ::viam::component::arm::v1::MoveThroughJointPositionsStreamedResponse,
        ::viam::component::arm::v1::MoveThroughJointPositionsStreamedRequest>* stream) noexcept {
    using request_type = ::viam::component::arm::v1::MoveThroughJointPositionsStreamedRequest;

    // TODO(RSDK-14164): like `ArmClient`, this hand-rolls the BiDi stream,
    // pending a shared BiDi client/server helper (RSDK-14164).
    //
    // The point of the BiDi stream is that an arm-side fault can reach the client
    // mid-execution rather than only at the end. This dispatcher runs the
    // implementation on the gRPC handler thread, with `batch_source` blocking in
    // `stream->Read` and `update_handler` in `stream->Write`, and no background
    // reader. The cost, specific to the synchronous C++ SDK, is that while the
    // implementation is parked in a `batch_source` `Read` it can't send an
    // update, so a fault reaches the client only on the next inbound batch, or
    // promptly once the client half-closes its send side (a client that stops
    // sending without half-closing, then sits idle, can hold onto an unreported
    // fault). The correct fix would require an async C++ SDK, and can wait until
    // `trajectory_update` carries data that must flow while the client is still
    // sending.
    ServerSpanGuard span_guard{context, "ArmServer::MoveThroughJointPositionsStreamed"};

    // The stream must start with exactly one `Init` message.
    request_type first;
    if (!stream->Read(&first)) {
        return span_guard.commit(
            ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                           "MoveThroughJointPositionsStreamed: stream closed before Init message"));
    }
    if (first.message_case() != request_type::kInit) {
        return span_guard.commit(
            ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                           "MoveThroughJointPositionsStreamed: first message must be Init"));
    }
    const auto& init = first.init();

    // Everything from here on can throw: the resource lookup takes a lock, proto
    // conversion allocates, the impl runs, and the `batch_source` closure throws
    // `grpc::Status` on a protocol violation. We are noexcept, so it all goes in
    // one try that catches every throw type.
    try {
        // Resolve the arm before anything else, since nothing downstream matters
        // if the named resource is not here. This mirrors `ServiceHelper`, which
        // looks the resource up before running the method body and fails
        // `NOT_FOUND` when the name does not resolve (it does not separately
        // special-case a missing name).
        const auto arm = resource_manager()->resource<Arm>(first.name());
        if (!arm) {
            return span_guard.commit(::grpc::Status(
                ::grpc::StatusCode::NOT_FOUND,
                "MoveThroughJointPositionsStreamed: arm not found: " + first.name()));
        }

        const ProtoStruct extra = from_proto(init.extra());

        // `batch_source` pulls the next batch off the stream. An empty batch is
        // tolerated as a no-op (the impl never sees it); a second `Init`, or a
        // message with no oneof set, is a protocol violation and gets a thrown
        // `grpc::Status` so the client sees a clear `INVALID_ARGUMENT`. Each point
        // is validated as it arrives, and the validator lives across calls so it
        // can enforce the stream-wide strictly-increasing-time rule. The server
        // always validates, since it can't trust that the caller did.
        //
        // A cancelled context and a false `Read` both return `boost::none`, and
        // the impl can't tell them apart: either way no more batches are coming,
        // and all it can do is stop pulling and finish what it has. `Read` goes
        // false both on a clean client half-close and on a broken or cancelled
        // stream, and sorting out which is gRPC's problem (via the terminal
        // status), not the impl's.
        auto batch_source = [stream, context, validator = TrajectoryStreamValidator{}]() mutable
            -> boost::optional<std::vector<Arm::trajectory_point>> {
            while (true) {
                if (context->IsCancelled()) {
                    return boost::none;
                }

                request_type msg;
                if (!stream->Read(&msg)) {
                    return boost::none;
                }

                const auto mc = msg.message_case();
                if (mc == request_type::kInit) {
                    throw ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                                         "MoveThroughJointPositionsStreamed: Init may only "
                                         "appear as the first message");
                }
                if (mc != request_type::kBatch) {
                    throw ::grpc::Status(
                        ::grpc::StatusCode::INVALID_ARGUMENT,
                        "MoveThroughJointPositionsStreamed: expected TrajectoryBatch");
                }

                std::vector<Arm::trajectory_point> batch;
                batch.reserve(msg.batch().points_size());
                for (const auto& pb_point : msg.batch().points()) {
                    auto point = from_proto(pb_point);
                    if (const auto err = validator.check(point)) {
                        throw ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                                             "MoveThroughJointPositionsStreamed: " + *err);
                    }
                    batch.push_back(std::move(point));
                }

                if (!batch.empty()) {
                    return batch;
                }
            }
        };

        // `update_handler` serializes one update onto the wire, converting it
        // through `to_proto` so the wire payload tracks `trajectory_update` as it
        // grows fields. Returns false on cancellation or wire failure, so the
        // impl knows to stop.
        auto update_handler = [stream, context](Arm::trajectory_update update) {
            if (context->IsCancelled()) {
                return false;
            }
            return stream->Write(to_proto(update));
        };

        // Turn on the thread-local context observer so the impl can read incoming
        // gRPC metadata (e.g. `viam_client`) and check cancellation via
        // `GrpcContextObserver::current()`, the same setup `ServiceHelper` does
        // for the non-streaming methods.
        const GrpcContextObserver::Enable observer_enable{*context};

        // Call the virtual. Its `stream_outcome` is meaningful only to a direct
        // caller of the impl; over the wire the client reconstructs its own
        // outcome, so the dispatcher intentionally discards it here.
        arm->move_through_joint_positions_streamed(batch_source, update_handler, extra);
    } catch (const ::grpc::Status& s) {
        return span_guard.commit(s);
    } catch (const std::invalid_argument& e) {
        return span_guard.commit(::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, e.what()));
    } catch (const std::exception& e) {
        return span_guard.commit(::grpc::Status(::grpc::StatusCode::INTERNAL, e.what()));
    } catch (...) {
        return span_guard.commit(::grpc::Status(
            ::grpc::StatusCode::INTERNAL, "MoveThroughJointPositionsStreamed: unknown exception"));
    }
    return span_guard.commit(::grpc::Status::OK);
}

::grpc::Status ArmServer::Stop(::grpc::ServerContext* context,
                               const ::viam::component::arm::v1::StopRequest* request,
                               ::viam::component::arm::v1::StopResponse*) noexcept {
    return make_service_helper<Arm>("ArmServer::Stop", this, context, request)(
        [&](auto& helper, auto& arm) { arm->stop(helper.getExtra()); });
}

::grpc::Status ArmServer::IsMoving(
    ::grpc::ServerContext* context,
    const ::viam::component::arm::v1::IsMovingRequest* request,
    ::viam::component::arm::v1::IsMovingResponse* response) noexcept {
    return make_service_helper<Arm>("ArmServer::IsMoving", this, context, request)(
        [&](auto&, auto& arm) { response->set_is_moving(arm->is_moving()); });
}

::grpc::Status ArmServer::DoCommand(::grpc::ServerContext* context,
                                    const ::viam::common::v1::DoCommandRequest* request,
                                    ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Arm>(
        "ArmServer::DoCommand", this, context, request)([&](auto&, auto& arm) {
        const ProtoStruct result = arm->do_command(from_proto(request->command()));
        *response->mutable_result() = to_proto(result);
    });
}

::grpc::Status ArmServer::GetStatus(::grpc::ServerContext* context,
                                    const ::viam::common::v1::GetStatusRequest* request,
                                    ::viam::common::v1::GetStatusResponse* response) noexcept {
    return make_service_helper<Arm>(
        "ArmServer::GetStatus", this, context, request)([&](auto&, auto& arm) {
        const ProtoStruct result = arm->get_status();
        *response->mutable_result() = to_proto(result);
    });
}

::grpc::Status ArmServer::GetKinematics(
    ::grpc::ServerContext* context,
    const ::viam::common::v1::GetKinematicsRequest* request,
    ::viam::common::v1::GetKinematicsResponse* response) noexcept {
    return make_service_helper<Arm>(
        "ArmServer::GetKinematics", this, context, request)([&](auto& helper, auto& arm) {
        *response = to_proto(arm->get_kinematics(helper.getExtra()));
    });
}

::grpc::Status ArmServer::Get3DModels(::grpc::ServerContext* context,
                                      const ::viam::common::v1::Get3DModelsRequest* request,
                                      ::viam::common::v1::Get3DModelsResponse* response) noexcept {
    return make_service_helper<Arm>(
        "ArmServer::Get3DModels", this, context, request)([&](auto& helper, auto& arm) {
        const std::map<std::string, mesh> models = arm->get_3d_models(helper.getExtra());

        for (const auto& entry : models) {
            response->mutable_models()->insert({entry.first, to_proto(entry.second)});
        }
    });
}

::grpc::Status ArmServer::GetGeometries(
    ::grpc::ServerContext* context,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<Arm>(
        "ArmServer::GetGeometries", this, context, request)([&](auto& helper, auto& arm) {
        const std::vector<GeometryConfig> geometries = arm->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = to_proto(geometry);
        }
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
