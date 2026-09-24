#include <viam/sdk/tests/mocks/mock_ml_training.hpp>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {
namespace ml_training {

using namespace viam::sdk;

MLTrainingService::Container fake_container() {
    MLTrainingService::Container container;
    container.id = "test_id";
    container.organization_id = "test_org_id";
    container.image_uri = "test_image_uri";
    container.description = "test_description";
    container.visibility = "public";
    return container;
}

std::unordered_map<std::string, MLTrainingService::Container> fake_supported_containers() {
    std::unordered_map<std::string, MLTrainingService::Container> containers;
    containers["test_container_name"] = fake_container();
    return containers;
}

std::vector<MLTrainingService::Container> fake_containers() {
    std::vector<MLTrainingService::Container> containers;
    containers.push_back(fake_container());
    return containers;
}

std::shared_ptr<MockMLTrainingService> MockMLTrainingService::get_mock_ml_training_service() {
    auto service = std::make_shared<MockMLTrainingService>("mock_ml_training_service");
    service->mock_supported_containers = fake_supported_containers();
    service->mock_containers = fake_containers();
    service->mock_registered_container_id = "new_container_id";
    return service;
}

std::unordered_map<std::string, MLTrainingService::Container>
MockMLTrainingService::list_supported_containers(const ProtoStruct& extra) {
    peek_extra = extra;
    return mock_supported_containers;
}

std::vector<MLTrainingService::Container>
MockMLTrainingService::list_containers(const std::string& organization_id,
                                       const ProtoStruct& extra) {
    peek_organization_id = organization_id;
    peek_extra = extra;
    return mock_containers;
}

std::string MockMLTrainingService::register_custom_training_container(
    const std::string& organization_id, const std::string& image_uri, const std::string& description,
    const ProtoStruct& extra) {
    peek_organization_id = organization_id;
    peek_image_uri = image_uri;
    peek_description = description;
    peek_extra = extra;
    return mock_registered_container_id;
}

void MockMLTrainingService::delete_custom_training_container(const std::string& id,
                                                             const ProtoStruct& extra) {
    peek_container_id = id;
    peek_extra = extra;
}

}  // namespace ml_training
}  // namespace sdktests
}  // namespace viam
