#include <viam/sdk/app/viam_client.hpp>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

ViamClient ViamClient::from_env() {
    const std::string api_key = get_env("VIAM_API_KEY");
    const std::string api_key_id = get_env("VIAM_API_KEY_ID");

    if (api_key.empty() || api_key_id.empty()) {
        throw Exception("VIAM_API_KEY and VIAM_API_KEY_ID must both be set");
    }

    ViamChannel::Options opts;
    opts.set_credentials(Credentials("api-key", api_key));
    opts.set_webrtc_disabled(true);
    opts.set_entity(api_key_id);

    return ViamClient(ViamChannel::dial_direct("app.viam.com:443", opts));
}

const ViamChannel& ViamClient::channel() const {
    return channel_;
}

ViamClient::ViamClient(ViamChannel channel) : channel_(std::move(channel)) {}

}  // namespace sdk
}  // namespace viam
