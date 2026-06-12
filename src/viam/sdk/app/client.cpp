#include <grpcpp/client_context.h>
#include <grpcpp/grpc.h>
#include <grpcpp/status.h>

#include <viam/sdk/app/app.hpp>
#include <viam/sdk/app/app_client.hpp>
#include <viam/sdk/app/app_proto.hpp>
#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/utils.hpp> // For AttributeMap conversions

namespace viam {
namespace sdk {

using grpc::ClientContext;
using grpc::Status;

AppClient::AppClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(viam::app::v1::App::NewStub(channel)) {}

AppClient::AppClient(std::shared_ptr<grpc::Channel> channel, const std::string& auth_token)
    : AppClient(channel) {
    auth_token_ = auth_token;
}

CreateAppResponse AppClient::CreateApp(const CreateAppRequest& request, const AttributeMap& extra) {
    viam::app::v1::CreateAppRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::CreateAppResponse proto_response;
    const Status status = stub_->CreateApp(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

DeleteAppResponse AppClient::DeleteApp(const DeleteAppRequest& request, const AttributeMap& extra) {
    viam::app::v1::DeleteAppRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::DeleteAppResponse proto_response;
    const Status status = stub_->DeleteApp(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

ListAppsResponse AppClient::ListApps(const ListAppsRequest& request, const AttributeMap& extra) {
    viam::app::v1::ListAppsRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::ListAppsResponse proto_response;
    const Status status = stub_->ListApps(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

CreateModuleResponse AppClient::CreateModule(const CreateModuleRequest& request,
                                           const AttributeMap& extra) {
    viam::app::v1::CreateModuleRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::CreateModuleResponse proto_response;
    const Status status = stub_->CreateModule(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

DeleteModuleResponse AppClient::DeleteModule(const DeleteModuleRequest& request,
                                           const AttributeMap& extra) {
    viam::app::v1::DeleteModuleRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::DeleteModuleResponse proto_response;
    const Status status = stub_->DeleteModule(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

ListModulesResponse AppClient::ListModules(const ListModulesRequest& request,
                                         const AttributeMap& extra) {
    viam::app::v1::ListModulesRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::ListModulesResponse proto_response;
    const Status status = stub_->ListModules(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

UpdateModuleResponse AppClient::UpdateModule(const UpdateModuleRequest& request,
                                           const AttributeMap& extra) {
    viam::app::v1::UpdateModuleRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::UpdateModuleResponse proto_response;
    const Status status = stub_->UpdateModule(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

CreateLocationResponse AppClient::CreateLocation(const CreateLocationRequest& request,
                                               const AttributeMap& extra) {
    viam::app::v1::CreateLocationRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::CreateLocationResponse proto_response;
    const Status status = stub_->CreateLocation(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

DeleteLocationResponse AppClient::DeleteLocation(const DeleteLocationRequest& request,
                                               const AttributeMap& extra) {
    viam::app::v1::DeleteLocationRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::DeleteLocationResponse proto_response;
    const Status status = stub_->DeleteLocation(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

ListLocationsResponse AppClient::ListLocations(const ListLocationsRequest& request,
                                               const AttributeMap& extra) {
    viam::app::v1::ListLocationsRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::ListLocationsResponse proto_response;
    const Status status = stub_->ListLocations(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

GetLocationResponse AppClient::GetLocation(const GetLocationRequest& request,
                                         const AttributeMap& extra) {
    viam::app::v1::GetLocationRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::GetLocationResponse proto_response;
    const Status status = stub_->GetLocation(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

UpdateLocationResponse AppClient::UpdateLocation(const UpdateLocationRequest& request,
                                                 const AttributeMap& extra) {
    viam::app::v1::UpdateLocationRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::UpdateLocationResponse proto_response;
    const Status status = stub_->UpdateLocation(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

CreateFragmentResponse AppClient::CreateFragment(const CreateFragmentRequest& request,
                                                 const AttributeMap& extra) {
    viam::app::v1::CreateFragmentRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::CreateFragmentResponse proto_response;
    const Status status = stub_->CreateFragment(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

DeleteFragmentResponse AppClient::DeleteFragment(const DeleteFragmentRequest& request,
                                                 const AttributeMap& extra) {
    viam::app::v1::DeleteFragmentRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::DeleteFragmentResponse proto_response;
    const Status status = stub_->DeleteFragment(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

ListFragmentsResponse AppClient::ListFragments(const ListFragmentsRequest& request,
                                               const AttributeMap& extra) {
    viam::app::v1::ListFragmentsRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::ListFragmentsResponse proto_response;
    const Status status = stub_->ListFragments(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

GetFragmentResponse AppClient::GetFragment(const GetFragmentRequest& request,
                                           const AttributeMap& extra) {
    viam::app::v1::GetFragmentRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::GetFragmentResponse proto_response;
    const Status status = stub_->GetFragment(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

UpdateFragmentResponse AppClient::UpdateFragment(const UpdateFragmentRequest& request,
                                                 const AttributeMap& extra) {
    viam::app::v1::UpdateFragmentRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::UpdateFragmentResponse proto_response;
    const Status status = stub_->UpdateFragment(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

CreateRegistryItemResponse AppClient::CreateRegistryItem(const CreateRegistryItemRequest& request,
                                                       const AttributeMap& extra) {
    viam::app::v1::CreateRegistryItemRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::CreateRegistryItemResponse proto_response;
    const Status status = stub_->CreateRegistryItem(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

DeleteRegistryItemResponse AppClient::DeleteRegistryItem(const DeleteRegistryItemRequest& request,
                                                       const AttributeMap& extra) {
    viam::app::v1::DeleteRegistryItemRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::DeleteRegistryItemResponse proto_response;
    const Status status = stub_->DeleteRegistryItem(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

ListRegistryItemsResponse AppClient::ListRegistryItems(const ListRegistryItemsRequest& request,
                                                     const AttributeMap& extra) {
    viam::app::v1::ListRegistryItemsRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::ListRegistryItemsResponse proto_response;
    const Status status = stub_->ListRegistryItems(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

GetRegistryItemResponse AppClient::GetRegistryItem(const GetRegistryItemRequest& request,
                                                   const AttributeMap& extra) {
    viam::app::v1::GetRegistryItemRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::GetRegistryItemResponse proto_response;
    const Status status = stub_->GetRegistryItem(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

UpdateRegistryItemResponse AppClient::UpdateRegistryItem(const UpdateRegistryItemRequest& request,
                                                       const AttributeMap& extra) {
    viam::app::v1::UpdateRegistryItemRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::UpdateRegistryItemResponse proto_response;
    const Status status = stub_->UpdateRegistryItem(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

TransferRegistryItemResponse AppClient::TransferRegistryItem(
    const TransferRegistryItemRequest& request, const AttributeMap& extra) {
    viam::app::v1::TransferRegistryItemRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    add_auth_token(ctx);
    viam::app::v1::TransferRegistryItemResponse proto_response;
    const grpc::Status status =
        stub_->TransferRegistryItem(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

DeprecateRegistryItemResponse AppClient::DeprecateRegistryItem(
    const DeprecateRegistryItemRequest& request, const AttributeMap& extra) {
    viam::app::v1::DeprecateRegistryItemRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    viam::app::v1::DeprecateRegistryItemResponse proto_response;
    const grpc::Status status =
        stub_->DeprecateRegistryItem(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

UndeprecateRegistryItemResponse AppClient::UndeprecateRegistryItem(
    const UndeprecateRegistryItemRequest& request, const AttributeMap& extra) {
    viam::app::v1::UndeprecateRegistryItemRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    viam::app::v1::UndeprecateRegistryItemResponse proto_response;
    const grpc::Status status =
        stub_->UndeprecateRegistryItem(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

DeprecateRegistryItemVersionResponse AppClient::DeprecateRegistryItemVersion(
    const DeprecateRegistryItemVersionRequest& request, const AttributeMap& extra) {
    viam::app::v1::DeprecateRegistryItemVersionRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    viam::app::v1::DeprecateRegistryItemVersionResponse proto_response;
    const grpc::Status status =
        stub_->DeprecateRegistryItemVersion(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

UndeprecateRegistryItemVersionResponse AppClient::UndeprecateRegistryItemVersion(
    const UndeprecateRegistryItemVersionRequest& request, const AttributeMap& extra) {
    viam::app::v1::UndeprecateRegistryItemVersionRequest proto_request = to_proto(request);
    *proto_request.mutable_extra() = to_proto(extra);
    ClientContext ctx;
    viam::app::v1::UndeprecateRegistryItemVersionResponse proto_response;
    const grpc::Status status =
        stub_->UndeprecateRegistryItemVersion(&ctx, proto_request, &proto_response);
    if (!status.ok()) {
        throw GRPCException(&status);
    }
    return from_proto(proto_response);
}

}  // namespace sdk
}  // namespace viam
