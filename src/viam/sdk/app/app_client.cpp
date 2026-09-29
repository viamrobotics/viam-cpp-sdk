#include <viam/sdk/app/app_client.hpp>

#include <grpcpp/channel.h>

#include <viam/api/app/v1/app.grpc.pb.h>
#include <viam/api/app/v1/app.pb.h>

#include <viam/sdk/common/client_helper.hpp>

namespace viam {
namespace sdk {

using viam::app::v1::AppService;

namespace {

AppClient::favorite_machine favorite_machine_from_proto(const app::v1::FavoriteMachine& proto) {
    return {proto.machine_id(), proto.organization_id(), from_proto(proto.created_on())};
}

}  // namespace

struct AppClient::impl {
    impl(const ViamChannel& c) : chan(&c), stub(AppService::NewStub(c.channel())) {}

    const ViamChannel& channel() const {
        return *chan;
    }

    template <typename Method>
    auto client_helper(Method m) {
        return make_client_helper(this, *stub, m);
    }

    const ViamChannel* chan;
    std::unique_ptr<AppService::Stub> stub;
};

AppClient AppClient::from_viam_client(const ViamClient& client) {
    return AppClient(client.channel());
}

AppClient::AppClient(const ViamChannel& channel) : pimpl_(std::make_unique<impl>(channel)) {}

AppClient::AppClient(AppClient&&) noexcept = default;
AppClient& AppClient::operator=(AppClient&&) noexcept = default;

AppClient::~AppClient() = default;

const ViamChannel& AppClient::channel() const {
    return pimpl_->channel();
}

AppClient::favorite_machine AppClient::add_favorite_machine(const std::string& machine_id) {
    return pimpl_->client_helper(&AppService::Stub::AddFavoriteMachine)
        .with([&](app::v1::AddFavoriteMachineRequest& req) {
            req.set_machine_id(machine_id);
        })
        .invoke([](const app::v1::AddFavoriteMachineResponse& resp) {
            return favorite_machine_from_proto(resp.favorite());
        });
}

void AppClient::remove_favorite_machine(const std::string& machine_id) {
    pimpl_->client_helper(&AppService::Stub::RemoveFavoriteMachine)
        .with([&](app::v1::RemoveFavoriteMachineRequest& req) {
            req.set_machine_id(machine_id);
        })
        .invoke();
}

std::vector<AppClient::favorite_machine> AppClient::list_favorite_machines() {
    return pimpl_->client_helper(&AppService::Stub::ListFavoriteMachines)
        .invoke([](const app::v1::ListFavoriteMachinesResponse& resp) {
            std::vector<favorite_machine> result;
            result.reserve(resp.favorites_size());

            for (const auto& fav : resp.favorites()) {
                result.push_back(favorite_machine_from_proto(fav));
            }

            return result;
        });
}

}  // namespace sdk
}  // namespace viam
