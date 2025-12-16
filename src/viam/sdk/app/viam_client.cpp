#include <viam/sdk/app/viam_client.hpp>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

ViamClient ViamClient::from_env(const char* uri) {
    boost::optional<std::string> api_key = get_env("VIAM_API_KEY");
    boost::optional<std::string> api_key_id = get_env("VIAM_API_KEY_ID");

    if (!api_key) {
        throw Exception("VIAM_API_KEY must be set");
    }

    if (!api_key_id) {
        throw Exception("VIAM_API_KEY_ID must be set");
    }

    ViamChannel::Options opts;
    opts.set_credentials(Credentials("api-key", std::move(*api_key)));
    opts.set_webrtc_disabled(true);
    opts.set_entity(std::move(api_key_id));

    return ViamClient(ViamChannel::dial_direct(uri, opts));
}

ViamClient ViamClient::from_env() {
    return ViamClient::from_env("app.viam.com");
}

const ViamChannel& ViamClient::channel() const {
    return channel_;
}

ViamClient::ViamClient(ViamChannel channel) : channel_(std::move(channel)) {}

}  // namespace sdk
}  // namespace viam
