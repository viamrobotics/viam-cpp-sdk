#include <viam/api/app/v1/app.grpc.pb.h>
#include <viam/api/app/v1/app.pb.h>
#include <viam/sdk/common/proto_convert.hpp>
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

ViamClient::ViamClient(ViamChannel channel) : channel_(std::move(channel)), app_service_stub_(viam::app::v1::AppService::NewStub(channel_.channel())) {}

void ViamClient::UploadDevicePushToken(const std::string& app_id,
                                       const std::string& device_token,
                                       const std::string& device_uuid,
                                       const RPCSubtype& subtype) {
    viam::app::v1::UploadDevicePushTokenRequest request;
    request.set_app_id(app_id);
    request.set_device_token(device_token);
    request.set_device_uuid(device_uuid);
    *request.mutable_extra() = to_proto(subtype);
    make_client_helper(this, *app_service_stub_, &viam::app::v1::AppService::Stub::UploadDevicePushToken)
        .with([&](auto& req) {
            req = request;
        })
        .invoke();
}

void ViamClient::DeleteDevicePushToken(const std::string& app_id,
                                       const std::string& device_uuid,
                                       const RPCSubtype& subtype) {
    viam::app::v1::DeleteDevicePushTokenRequest request;
    request.set_app_id(app_id);
    request.set_device_uuid(device_uuid);
    *request.mutable_extra() = to_proto(subtype);
    make_client_helper(this, *app_service_stub_, &viam::app::v1::AppService::Stub::DeleteDevicePushToken)
        .with([&](auto& req) {
            req = request;
        })
        .invoke();
}

std::vector<std::string> ViamClient::GetDevicePushTokens(const std::string& app_id,
                                                         const RPCSubtype& subtype) {
    viam::app::v1::GetDevicePushTokensRequest request;
    request.set_app_id(app_id);
    *request.mutable_extra() = to_proto(subtype);
    return make_client_helper(this, *app_service_stub_, &viam::app::v1::AppService::Stub::GetDevicePushTokens)
        .with([&](auto& req) {
            req = request;
        })
        .invoke([](const viam::app::v1::GetDevicePushTokensResponse& resp) {
            std::vector<std::string> device_tokens;
            for (const auto& token : resp.device_tokens()) {
                device_tokens.push_back(token);
            }
            return device_tokens;
        });
}

void ViamClient::SetFirebaseConfig(const std::string& org_id,
                                   const std::string& app_id,
                                   const std::string& config_json,
                                   const RPCSubtype& subtype) {
    viam::app::v1::SetFirebaseConfigRequest request;
    request.set_org_id(org_id);
    request.set_app_id(app_id);
    request.set_config_json(config_json);
    *request.mutable_extra() = to_proto(subtype);
    make_client_helper(this, *app_service_stub_, &viam::app::v1::AppService::Stub::SetFirebaseConfig)
        .with([&](auto& req) {
            req = request;
        })
        .invoke();
}

std::string ViamClient::GetFirebaseConfig(const std::string& org_id,
                                          const RPCSubtype& subtype) {
    viam::app::v1::GetFirebaseConfigRequest request;
    request.set_org_id(org_id);
    *request.mutable_extra() = to_proto(subtype);
    return make_client_helper(this, *app_service_stub_, &viam::app::v1::AppService::Stub::GetFirebaseConfig)
        .with([&](auto& req) {
            req = request;
        })
        .invoke([](const viam::app::v1::GetFirebaseConfigResponse& resp) {
            return resp.app_id();
        });
}

void ViamClient::DeleteFirebaseConfig(const std::string& org_id,
                                      const std::string& app_id,
                                      const RPCSubtype& subtype) {
    viam::app::v1::DeleteFirebaseConfigRequest request;
    request.set_org_id(org_id);
    request.set_app_id(app_id);
    *request.mutable_extra() = to_proto(subtype);
    make_client_helper(this, *app_service_stub_, &viam::app::v1::AppService::Stub::DeleteFirebaseConfig)
        .with([&](auto& req) {
            req = request;
        })
        .invoke();
}

}  // namespace sdk
}  // namespace viam