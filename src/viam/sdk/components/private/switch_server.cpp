#include <viam/sdk/components/private/switch_server.hpp>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/private/service_helper.hpp>

namespace viam {
namespace sdk {
namespace impl {

SwitchServer::SwitchServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status SwitchServer::SetPosition(
    ::grpc::ServerContext* context,
    const ::viam::component::switch_::v1::SetPositionRequest* request,
    ::viam::component::switch_::v1::SetPositionResponse*) noexcept {
    return make_service_helper<Switch>(
        "SwitchServer::SetPosition", this, context, request)([&](auto& helper, auto& switch_) {
        switch_->set_position(request->position(), helper.getExtra());
    });
}

::grpc::Status SwitchServer::GetPosition(
    ::grpc::ServerContext* context,
    const ::viam::component::switch_::v1::GetPositionRequest* request,
    ::viam::component::switch_::v1::GetPositionResponse* response) noexcept {
    return make_service_helper<Switch>(
        "SwitchServer::GetPosition", this, context, request)([&](auto& helper, auto& switch_) {
        response->set_position(switch_->get_position(helper.getExtra()));
    });
}

::grpc::Status SwitchServer::GetNumberOfPositions(
    ::grpc::ServerContext* context,
    const ::viam::component::switch_::v1::GetNumberOfPositionsRequest* request,
    ::viam::component::switch_::v1::GetNumberOfPositionsResponse* response) noexcept {
    return make_service_helper<Switch>(
        "SwitchServer::GetNumberOfPositions", this, context, request)(
        [&](auto& helper, auto& switch_) {
            const auto info = switch_->get_number_of_positions(helper.getExtra());
            if (!info.position_labels.empty() &&
                static_cast<uint32_t>(info.position_labels.size()) != info.num_positions) {
                throw Exception(
                    "get_number_of_positions: labels size does not match num_positions");
            }
            response->set_number_of_positions(info.num_positions);
            for (const auto& label : info.position_labels) {
                response->add_labels(label);
            }
        });
}

::grpc::Status SwitchServer::DoCommand(::grpc::ServerContext* context,
                                       const ::viam::common::v1::DoCommandRequest* request,
                                       ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Switch>(
        "SwitchServer::DoCommand", this, context, request)([&](auto&, auto& switch_) {
        const ProtoStruct result = switch_->do_command(from_proto(request->command()));
        *response->mutable_result() = to_proto(result);
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
