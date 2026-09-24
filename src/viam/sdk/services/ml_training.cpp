#include <viam/sdk/services/ml_training.hpp>

#include <viam/api/app/mltraining/v1/ml_training.pb.h>
#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

MLTrainingService::MLTrainingService(std::string name) : Service(std::move(name)) {}

API MLTrainingService::api() const {
    return API::get<MLTrainingService>();
}

API API::traits<MLTrainingService>::api() {
    return {kRDK, kService, "mltraining"};
}

bool operator==(const MLTrainingService::Container& lhs, const MLTrainingService::Container& rhs) {
    return lhs.id == rhs.id && lhs.organization_id == rhs.organization_id &&
           lhs.image_uri == rhs.image_uri && lhs.description == rhs.description &&
           lhs.visibility == rhs.visibility;
}

namespace proto_convert_details {

void to_proto_impl<MLTrainingService::Container>::operator()(
    const MLTrainingService::Container& self, viam::app::mltraining::v1::Container* proto) const {
    proto->set_id(self.id);
    proto->set_organization_id(self.organization_id);
    proto->set_image_uri(self.image_uri);
    proto->set_description(self.description);
    proto->set_visibility(self.visibility);
}

MLTrainingService::Container from_proto_impl<viam::app::mltraining::v1::Container>::operator()(
    const viam::app::mltraining::v1::Container* proto) const {
    MLTrainingService::Container result;
    result.id = proto->id();
    result.organization_id = proto->organization_id();
    result.image_uri = proto->image_uri();
    result.description = proto->description();
    result.visibility = proto->visibility();
    return result;
}

void to_proto_impl<MLTrainingService::ListContainersResponse>::operator()(
    const MLTrainingService::ListContainersResponse& self,
    viam::app::mltraining::v1::ListContainersResponse* proto) const {
    for (const auto& container : self.containers) {
        *proto->add_containers() = to_proto(container);
    }
}

MLTrainingService::ListContainersResponse
from_proto_impl<viam::app::mltraining::v1::ListContainersResponse>::operator()(
    const viam::app::mltraining::v1::ListContainersResponse* proto) const {
    MLTrainingService::ListContainersResponse result;
    for (const auto& container_proto : proto->containers()) {
        result.containers.push_back(from_proto(container_proto));
    }
    return result;
}

void to_proto_impl<MLTrainingService::RegisterCustomTrainingContainerResponse>::operator()(
    const MLTrainingService::RegisterCustomTrainingContainerResponse& self,
    viam::app::mltraining::v1::RegisterCustomTrainingContainerResponse* proto) const {
    proto->set_id(self.id);
}

MLTrainingService::RegisterCustomTrainingContainerResponse
from_proto_impl<viam::app::mltraining::v1::RegisterCustomTrainingContainerResponse>::operator()(
    const viam::app::mltraining::v1::RegisterCustomTrainingContainerResponse* proto) const {
    MLTrainingService::RegisterCustomTrainingContainerResponse result;
    result.id = proto->id();
    return result;
}

void to_proto_impl<MLTrainingService::DeleteCustomTrainingContainerResponse>::operator()(
    const MLTrainingService::DeleteCustomTrainingContainerResponse&,
    viam::app::mltraining::v1::DeleteCustomTrainingContainerResponse*) const {
    // No fields to set for an empty response.
}

MLTrainingService::DeleteCustomTrainingContainerResponse
from_proto_impl<viam::app::mltraining::v1::DeleteCustomTrainingContainerResponse>::operator()(
    const viam::app::mltraining::v1::DeleteCustomTrainingContainerResponse*) const {
    return {};
}

}  // namespace proto_convert_details

}  // namespace sdk
}  // namespace viam