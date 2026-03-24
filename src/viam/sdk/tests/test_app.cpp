#include <memory>
#include <string>
#include <vector>

#define BOOST_TEST_MODULE test module test_app
#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/app/viam_client.hpp>
#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/tests/test_utils.hpp>
#include <viam/sdk/tests/mocks/mock_app.hpp>

namespace viam {
namespace sdktests {
namespace app {

using namespace viam::sdk;

// A helper to set up a ViamClient connected to a MockAppService.
template <typename F>
void app_client_to_mocks_pipeline(F&& test_case) {
    auto server = std::make_shared<sdk::Server>();
    auto mock_app_service = std::make_shared<MockAppService>();
    server->register_service(mock_app_service.get());
    server->start();

    auto test_server = TestServer(server);
    auto grpc_channel = test_server.grpc_in_process_channel();
    auto client = ViamClient(ViamChannel(grpc_channel));

    std::forward<F>(test_case)(client, *mock_app_service);
}

BOOST_AUTO_TEST_SUITE(test_app)

BOOST_AUTO_TEST_CASE(test_upload_device_push_token) {
    app_client_to_mocks_pipeline(
        [](ViamClient& client, MockAppService& mock_app_service) -> void {
            const std::string app_id = "test_app_id";
            const std::string device_token = "test_device_token";
            const std::string device_uuid = "test_device_uuid";
            RPCSubtype subtype;
            subtype.emplace("key", "value");

            client.UploadDevicePushToken(app_id, device_token, device_uuid, subtype);

            BOOST_CHECK_EQUAL(mock_app_service.mock_upload_app_id, app_id);
            BOOST_CHECK_EQUAL(mock_app_service.mock_upload_device_token, device_token);
            BOOST_CHECK_EQUAL(mock_app_service.mock_upload_device_uuid, device_uuid);
        });
}

BOOST_AUTO_TEST_CASE(test_delete_device_push_token) {
    app_client_to_mocks_pipeline(
        [](ViamClient& client, MockAppService& mock_app_service) -> void {
            const std::string app_id = "test_app_id";
            const std::string device_uuid = "test_device_uuid";
            RPCSubtype subtype;
            subtype.emplace("key", "value");

            client.DeleteDevicePushToken(app_id, device_uuid, subtype);

            BOOST_CHECK_EQUAL(mock_app_service.mock_delete_app_id, app_id);
            BOOST_CHECK_EQUAL(mock_app_service.mock_delete_device_uuid, device_uuid);
        });
}

BOOST_AUTO_TEST_CASE(test_get_device_push_tokens) {
    app_client_to_mocks_pipeline(
        [](ViamClient& client, MockAppService& mock_app_service) -> void {
            const std::string app_id = "test_app_id";
            mock_app_service.mock_device_tokens_response = {"token1", "token2"};
            RPCSubtype subtype;
            subtype.emplace("key", "value");

            std::vector<std::string> tokens = client.GetDevicePushTokens(app_id, subtype);

            BOOST_CHECK_EQUAL(mock_app_service.mock_get_app_id, app_id);
            BOOST_CHECK_EQUAL(tokens.size(), 2);
            BOOST_CHECK_EQUAL(tokens[0], "token1");
            BOOST_CHECK_EQUAL(tokens[1], "token2");
        });
}

BOOST_AUTO_TEST_CASE(test_set_firebase_config) {
    app_client_to_mocks_pipeline(
        [](ViamClient& client, MockAppService& mock_app_service) -> void {
            const std::string org_id = "test_org_id";
            const std::string app_id = "test_app_id";
            const std::string config_json = "{\"key\": \"value\"}";
            RPCSubtype subtype;
            subtype.emplace("key", "value");

            client.SetFirebaseConfig(org_id, app_id, config_json, subtype);

            BOOST_CHECK_EQUAL(mock_app_service.mock_set_firebase_org_id, org_id);
            BOOST_CHECK_EQUAL(mock_app_service.mock_set_firebase_app_id, app_id);
            BOOST_CHECK_EQUAL(mock_app_service.mock_set_firebase_config_json, config_json);
        });
}

BOOST_AUTO_TEST_CASE(test_get_firebase_config) {
    app_client_to_mocks_pipeline(
        [](ViamClient& client, MockAppService& mock_app_service) -> void {
            const std::string org_id = "test_org_id";
            mock_app_service.mock_get_firebase_app_id_response = "returned_app_id";
            RPCSubtype subtype;
            subtype.emplace("key", "value");

            std::string app_id = client.GetFirebaseConfig(org_id, subtype);

            BOOST_CHECK_EQUAL(mock_app_service.mock_get_firebase_org_id, org_id);
            BOOST_CHECK_EQUAL(app_id, "returned_app_id");
        });
}

BOOST_AUTO_TEST_CASE(test_delete_firebase_config) {
    app_client_to_mocks_pipeline(
        [](ViamClient& client, MockAppService& mock_app_service) -> void {
            const std::string org_id = "test_org_id";
            const std::string app_id = "test_app_id";
            RPCSubtype subtype;
            subtype.emplace("key", "value");

            client.DeleteFirebaseConfig(org_id, app_id, subtype);

            BOOST_CHECK_EQUAL(mock_app_service.mock_delete_firebase_org_id, org_id);
            BOOST_CHECK_EQUAL(mock_app_service.mock_delete_firebase_app_id, app_id);
        });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace app
}  // namespace sdktests
}  // namespace viam