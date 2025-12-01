#include <viam/sdk/app/client.hpp>

#include <viam/sdk/common/exception.hpp>

namespace viam {
namespace sdk {

AppClient AppClient::connect(const char* uri,
                             const Credentials& credentials,
                             const std::string& auth_entity) {
    if (credentials.type() != "api-key") {
        throw Exception("AppClient connection requires API key");
    }

    ViamChannel::Options opts;
    opts.set_credentials(credentials);
    opts.set_webrtc_disabled(true);
    opts.set_entity(auth_entity);

    return AppClient(ViamChannel::dial_direct(uri, opts));
}

AppClient::AppClient(ViamChannel channel) : channel_(std::move(channel)) {}

}  // namespace sdk
}  // namespace viam
