#include <viam/sdk/app/client.hpp>

#include <viam/api/app/data/v1/data.grpc.pb.h>
#include <viam/api/app/data/v1/data.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

AppClient AppClient::from_env() {
    std::string api_key = get_env("VIAM_API_KEY");
    std::string api_key_id = get_env("VIAM_API_KEY_ID");

    if (api_key.empty() || api_key_id.empty()) {
        throw Exception("VIAM_API_KEY and VIAM_API_KEY_ID must both be set");
    }

    ViamChannel::Options opts;
    opts.set_credentials(Credentials("api-key", api_key));
    opts.set_webrtc_disabled(true);
    opts.set_entity(api_key_id);

    return AppClient(ViamChannel::dial_direct("app.viam.com:443", opts));
}

const ViamChannel& AppClient::channel() const {
    return channel_;
}

AppClient::AppClient(ViamChannel channel) : channel_(std::move(channel)) {}

}  // namespace sdk
}  // namespace viam
