#include <viam/sdk/tests/mocks/mock_app.hpp>

#include <grpcpp/grpcpp.h>

namespace viam {
namespace sdktests {
namespace app {

grpc::Status MockAppService::UploadDevicePushToken(
    grpc::ServerContext* context,
    const viam::app::v1::UploadDevicePushTokenRequest* request,
    viam::app::v1::UploadDevicePushTokenResponse* response) {
    mock_upload_app_id = request->app_id();
    mock_upload_device_token = request->device_token();
    mock_upload_device_uuid = request->device_uuid();
    return grpc::Status::OK;
}

grpc::Status MockAppService::DeleteDevicePushToken(
    grpc::ServerContext* context,
    const viam::app::v1::DeleteDevicePushTokenRequest* request,
    viam::app::v1::DeleteDevicePushTokenResponse* response) {
    mock_delete_app_id = request->app_id();
    mock_delete_device_uuid = request->device_uuid();
    return grpc::Status::OK;
}

grpc::Status MockAppService::GetDevicePushTokens(
    grpc::ServerContext* context,
    const viam::app::v1::GetDevicePushTokensRequest* request,
    viam::app::v1::GetDevicePushTokensResponse* response) {
    mock_get_app_id = request->app_id();
    for (const auto& token : mock_device_tokens_response) {
        response->add_device_tokens(token);
    }
    return grpc::Status::OK;
}

grpc::Status MockAppService::SetFirebaseConfig(
    grpc::ServerContext* context,
    const viam::app::v1::SetFirebaseConfigRequest* request,
    viam::app::v1::SetFirebaseConfigResponse* response) {
    mock_set_firebase_org_id = request->org_id();
    mock_set_firebase_app_id = request->app_id();
    mock_set_firebase_config_json = request->config_json();
    return grpc::Status::OK;
}

grpc::Status MockAppService::GetFirebaseConfig(
    grpc::ServerContext* context,
    const viam::app::v1::GetFirebaseConfigRequest* request,
    viam::app::v1::GetFirebaseConfigResponse* response) {
    mock_get_firebase_org_id = request->org_id();
    response->set_app_id(mock_get_firebase_app_id_response);
    return grpc::Status::OK;
}

grpc::Status MockAppService::DeleteFirebaseConfig(
    grpc::ServerContext* context,
    const viam::app::v1::DeleteFirebaseConfigRequest* request,
    viam::app::v1::DeleteFirebaseConfigResponse* response) {
    mock_delete_firebase_org_id = request->org_id();
    mock_delete_firebase_app_id = request->app_id();
    return grpc::Status::OK;
}

std::shared_ptr<MockAppService> MockAppService::get_mock_app() {
    return std::make_shared<MockAppService>();
}

}  // namespace app
}  // namespace sdktests
}  // namespace viam
