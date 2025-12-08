#include <viam/sdk/components/private/button_server.hpp>

#include <viam/sdk/common/private/service_helper.hpp>

namespace viam {
namespace sdk {
namespace impl {

ButtonServer::ButtonServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status ButtonServer::Push(::grpc::ServerContext* context,
                                  const ::viam::component::button::v1::PushRequest* request,
                                  ::viam::component::button::v1::PushResponse*) noexcept {
    return make_service_helper<Button>("ButtonServer::Push", this, context, request)(
        [&](auto& helper, auto& button) { button->push(helper.getExtra()); });
}

::grpc::Status ButtonServer::DoCommand(::grpc::ServerContext* context,
                                       const ::viam::common::v1::DoCommandRequest* request,
                                       ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<Button>(
        "ButtonServer::DoCommand", this, context, request)([&](auto&, auto& button) {
        const ProtoStruct result = button->do_command(from_proto(request->command()));
        *response->mutable_result() = to_proto(result);
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
