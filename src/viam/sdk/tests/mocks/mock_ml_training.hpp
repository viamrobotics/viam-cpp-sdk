#pragma once

#include <viam/sdk/services/ml_training.hpp>

namespace viam {
namespace sdktests {
namespace ml_training {

using namespace viam::sdk;

class MockMLTrainingService : public MLTrainingService {
   public:
    MockMLTrainingService(std::string name) : MLTrainingService(std::move(name)) {}

    static std::shared_ptr<MockMLTrainingService> get_mock_ml_training_service();

    std::unordered_map<std::string, Container> list_supported_containers(
        const ProtoStruct& extra) override;

    std::vector<Container> list_containers(const std::string& organization_id,
                                           const ProtoStruct& extra) override;

    std::string register_custom_training_container(const std::string& organization_id,
                                                   const std::string& image_uri,
                                                   const std::string& description,
                                                   const ProtoStruct& extra) override;

    void delete_custom_training_container(const std::string& id,
                                          const ProtoStruct& extra) override;

    // Peek variables for testing
    ProtoStruct peek_extra;
    std::string peek_organization_id;
    std::string peek_image_uri;
    std::string peek_description;
    std::string peek_container_id;

    std::unordered_map<std::string, Container> mock_supported_containers;
    std::vector<Container> mock_containers;
    std::string mock_registered_container_id;
};

MLTrainingService::Container fake_container();
std::unordered_map<std::string, MLTrainingService::Container> fake_supported_containers();
std::vector<MLTrainingService::Container> fake_containers();

}  // namespace ml_training
}  // namespace sdktests
}  // namespace viam