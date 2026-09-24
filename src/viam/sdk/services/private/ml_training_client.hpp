/// @file services/private/ml_training_client.hpp
///
/// @brief Implements a gRPC client for the `MLTrainingService` service.
#pragma once

#include <viam/api/app/mltraining/v1/ml_training.grpc.pb.h>

#include <viam/sdk/rpc/dial.hpp>
#include <viam/sdk/services/ml_training.hpp>

namespace viam {
namespace sdk {
namespace impl {

/// @class MLTrainingServiceClient
/// @brief gRPC client implementation of an `MLTrainingService` service.
/// @ingroup MLTrainingService
class MLTrainingServiceClient : public MLTrainingService {
   public:
    using interface_type = MLTrainingService;
    MLTrainingServiceClient(std::string name, const ViamChannel& channel);

    const ViamChannel& channel() const {
        return *channel_;
    }

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

    // The `extra` param is frequently unnecessary but needs to be supported. Ideally, we'd
    // like to live in a world where implementers of derived classes don't need to go out of
    // their way to support two versions of a method (an `extra` version and a non-`extra`
    // version), and users don't need to pass an unnecessary parameters to all method calls.
    //
    // To do this, we define in the parent resource class a non-virtual version of the methods
    // that calls the virtual method and passes a `nullptr` by default in place of the `extra`
    // param. In order to access these versions of the methods within the client code, however,
    // we need to include these `using` lines.
    using MLTrainingService::delete_custom_training_container;
    using MLTrainingService::list_containers;
    using MLTrainingService::list_supported_containers;
    using MLTrainingService::register_custom_training_container;

   private:
    using StubType = viam::app::mltraining::v1::MLTrainingService::StubInterface;
    std::unique_ptr<StubType> stub_;
    const ViamChannel* channel_;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam