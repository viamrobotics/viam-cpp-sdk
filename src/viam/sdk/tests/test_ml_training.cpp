#define BOOST_TEST_MODULE test module test_ml_training

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/services/ml_training.hpp>
#include <viam/sdk/tests/mocks/mock_ml_training.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace ml_training {

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_ml_training)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockMLTrainingService service("mock_ml_training_service");
    auto api = service.api();
    auto static_api = API::get<MLTrainingService>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "mltraining");
}

BOOST_AUTO_TEST_CASE(test_list_supported_containers) {
    auto mock = MockMLTrainingService::get_mock_ml_training_service();
    client_to_mock_pipeline<MLTrainingService>(mock, [&](MLTrainingService& client) {
        ProtoStruct extra;
        extra["foo"] = ProtoValue("bar");
        auto result = client.list_supported_containers(extra);

        BOOST_CHECK_EQUAL(result.size(), mock->mock_supported_containers.size());
        BOOST_CHECK_EQUAL(result.at("test_container_name").id,
                          mock->mock_supported_containers.at("test_container_name").id);
        BOOST_CHECK(mock->peek_extra.at("foo").is_a<std::string>());
        BOOST_CHECK_EQUAL(mock->peek_extra.at("foo").get_unchecked<std::string>(), "bar");
    });
}

BOOST_AUTO_TEST_CASE(test_list_containers) {
    auto mock = MockMLTrainingService::get_mock_ml_training_service();
    client_to_mock_pipeline<MLTrainingService>(mock, [&](MLTrainingService& client) {
        ProtoStruct extra;
        extra["foo"] = ProtoValue("bar");
        std::string org_id = "test_org_id";
        auto result = client.list_containers(org_id, extra);

        BOOST_CHECK_EQUAL(result.size(), mock->mock_containers.size());
        BOOST_CHECK_EQUAL(result[0].id, mock->mock_containers[0].id);
        BOOST_CHECK_EQUAL(mock->peek_organization_id, org_id);
        BOOST_CHECK(mock->peek_extra.at("foo").is_a<std::string>());
        BOOST_CHECK_EQUAL(mock->peek_extra.at("foo").get_unchecked<std::string>(), "bar");
    });
}

BOOST_AUTO_TEST_CASE(test_register_custom_training_container) {
    auto mock = MockMLTrainingService::get_mock_ml_training_service();
    client_to_mock_pipeline<MLTrainingService>(mock, [&](MLTrainingService& client) {
        ProtoStruct extra;
        extra["foo"] = ProtoValue("bar");
        std::string org_id = "test_org_id";
        std::string image_uri = "test_image_uri";
        std::string description = "test_description";
        std::string result_id = client.register_custom_training_container(
            org_id, image_uri, description, extra);

        BOOST_CHECK_EQUAL(result_id, mock->mock_registered_container_id);
        BOOST_CHECK_EQUAL(mock->peek_organization_id, org_id);
        BOOST_CHECK_EQUAL(mock->peek_image_uri, image_uri);
        BOOST_CHECK_EQUAL(mock->peek_description, description);
        BOOST_CHECK(mock->peek_extra.at("foo").is_a<std::string>());
        BOOST_CHECK_EQUAL(mock->peek_extra.at("foo").get_unchecked<std::string>(), "bar");
    });
}

BOOST_AUTO_TEST_CASE(test_delete_custom_training_container) {
    auto mock = MockMLTrainingService::get_mock_ml_training_service();
    client_to_mock_pipeline<MLTrainingService>(mock, [&](MLTrainingService& client) {
        ProtoStruct extra;
        extra["foo"] = ProtoValue("bar");
        std::string container_id = "container_to_delete";
        client.delete_custom_training_container(container_id, extra);

        BOOST_CHECK_EQUAL(mock->peek_container_id, container_id);
        BOOST_CHECK(mock->peek_extra.at("foo").is_a<std::string>());
        BOOST_CHECK_EQUAL(mock->peek_extra.at("foo").get_unchecked<std::string>(), "bar");
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace ml_training
}  // namespace sdktests
}  // namespace viam