#include <viam/sdk/app/viam_client.hpp>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <viam/api/app/v1/app.grpc.pb.h>
#include <viam/api/app/v1/app.pb.h>
#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/rpc/dial.hpp>

namespace viam {
namespace sdk {

ViamClient ViamClient::from_env(const char* uri) {
    return ViamClient(ViamChannel::from_env(uri));
}

ViamClient ViamClient::from_env() {
    return ViamClient(ViamChannel::from_env());
}

ViamClient::ViamClient(ViamChannel channel) : channel_(std::move(channel)) {}

const ViamChannel& ViamClient::channel() const {
    return channel_;
}

void ViamClient::UploadDevicePushToken(const std::string& app_id, const std::string& device_token, const std::string& device_uuid, const ProtoStruct& extra) {
    auto stub = viam::app::v1::AppService::NewStub(channel_.grpc_channel());
    make_client_helper(this, *stub, &viam::app::v1::AppService::Stub::UploadDevicePushToken)
        .with([&](viam::app::v1::UploadDevicePushTokenRequest& request) {
            request.set_app_id(app_id);
            request.set_device_token(device_token);
            request.set_device_uuid(device_uuid);
        })
        .with(extra)
        .invoke();
}

void ViamClient::DeleteDevicePushToken(const std::string& app_id, const std::string& device_uuid, const ProtoStruct& extra) {
    auto stub = viam::app::v1::AppService::NewStub(channel_.grpc_channel());
    make_client_helper(this, *stub, &viam::app::v1::AppService::Stub::DeleteDevicePushToken)
        .with([&](viam::app::v1::DeleteDevicePushTokenRequest& request) {
            request.set_app_id(app_id);
            request.set_device_uuid(device_uuid);
        })
        .with(extra)
        .invoke();
}

std::vector<std::string> ViamClient::GetDevicePushTokens(const std::string& app_id, const ProtoStruct& extra) {
    auto stub = viam::app::v1::AppService::NewStub(channel_.grpc_channel());
    return make_client_helper(this, *stub, &viam::app::v1::AppService::Stub::GetDevicePushTokens)
        .with([&](viam::app::v1::GetDevicePushTokensRequest& request) {
            request.set_app_id(app_id);
        })
        .with(extra)
        .invoke([](const viam::app::v1::GetDevicePushTokensResponse& response) {
            std::vector<std::string> device_tokens;
            for (const auto& token : response.device_tokens()) {
                device_tokens.push_back(token);
            }
            return device_tokens;
        });
}

void ViamClient::SetFirebaseConfig(const std::string& org_id, const std::string& app_id, const std::string& config_json, const ProtoStruct& extra) {
    auto stub = viam::app::v1::AppService::NewStub(channel_.grpc_channel());
    make_client_helper(this, *stub, &viam::app::v1::AppService::Stub::SetFirebaseConfig)
        .with([&](viam::app::v1::SetFirebaseConfigRequest& request) {
            request.set_org_id(org_id);
            request.set_app_id(app_id);
            request.set_config_json(config_json);
        })
        .with(extra)
        .invoke();
}

std::string ViamClient::GetFirebaseConfig(const std::string& org_id, const ProtoStruct& extra) {
    auto stub = viam::app::v1::AppService::NewStub(channel_.grpc_channel());
    return make_client_helper(this, *stub, &viam::app::v1::AppService::Stub::GetFirebaseConfig)
        .with([&](viam::app::v1::GetFirebaseConfigRequest& request) {
            request.set_org_id(org_id);
        })
        .with(extra)
        .invoke([](const viam::app::v1::GetFirebaseConfigResponse& response) {
            return response.app_id();
        });
}

void ViamClient::DeleteFirebaseConfig(const std::string& org_id, const std::string& app_id, const ProtoStruct& extra) {
    auto stub = viam::app::v1::AppService::NewStub(channel_.grpc_channel());
    make_client_helper(this, *stub, &viam::app::v1::AppService::Stub::DeleteFirebaseConfig)
        .with([&](viam::app::v1::DeleteFirebaseConfigRequest& request) {
            request.set_org_id(org_id);
            request.set_app_id(app_id);
        })
        .with(extra)
        .invoke();
}

}  // namespace sdk
}  // namespace viam