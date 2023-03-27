#include <common/v1/common.pb.h>
#include <component/camera/v1/camera.grpc.pb.h>
#include <component/camera/v1/camera.pb.h>

#include <components/generic/client.hpp>
#include <components/generic/generic.hpp>
#include <components/generic/server.hpp>
#include <tests/mocks/generic_mocks.hpp>
#include <tests/test_utils.hpp>

std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>
MockGeneric::do_command(
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>
        command) {
  return map;
}

std::shared_ptr<MockGeneric> MockGeneric::get_mock_generic() {
  std::shared_ptr<MockGeneric> generic = std::make_shared<MockGeneric>();
  generic->map = fake_map();

  return generic;
}

MockGenericStub::MockGenericStub()
    : server(GenericServer(std::make_shared<SubtypeService>())) {
  this->server.get_sub_svc()->add(std::string("generic"),
                                  MockGeneric::get_mock_generic());
}

::grpc::Status
MockGenericStub::DoCommand(::grpc::ClientContext *context,
                           const ::viam::common::v1::DoCommandRequest &request,
                           ::viam::common::v1::DoCommandResponse *response) {
  grpc::ServerContext *ctx;
  return server.DoCommand(ctx, &request, response);
}

std::unique_ptr<
    ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>>
MockGenericStub::AsyncDoCommand(
    ::grpc::ClientContext *context,
    const ::viam::common::v1::DoCommandRequest &request,
    ::grpc::CompletionQueue *cq) {
  return std::unique_ptr<
      ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>>(
      AsyncDoCommandRaw(context, request, cq));
}

std::unique_ptr<
    ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>>
MockGenericStub::PrepareAsyncDoCommand(
    ::grpc::ClientContext *context,
    const ::viam::common::v1::DoCommandRequest &request,
    ::grpc::CompletionQueue *cq) {
  return std::unique_ptr<
      ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>>(
      PrepareAsyncDoCommandRaw(context, request, cq));
}

void MockGenericStub::async::DoCommand(
    ::grpc::ClientContext *context,
    const ::viam::common::v1::DoCommandRequest *request,
    ::viam::common::v1::DoCommandResponse *response,
    std::function<void(::grpc::Status)>) {
  return;
}

void MockGenericStub::async::DoCommand(
    ::grpc::ClientContext *context,
    const ::viam::common::v1::DoCommandRequest *request,
    ::viam::common::v1::DoCommandResponse *response,
    ::grpc::ClientUnaryReactor *reactor) {
  return;
}

::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse> *
MockGenericStub::AsyncDoCommandRaw(
    ::grpc::ClientContext *context,
    const ::viam::common::v1::DoCommandRequest &request,
    ::grpc::CompletionQueue *cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
      ::viam::common::v1::DoCommandResponse,
      ::viam::common::v1::DoCommandRequest, ::grpc::protobuf::MessageLite,
      ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_DoCommand_,
                                     context, request);
}

::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse> *
MockGenericStub::PrepareAsyncDoCommandRaw(
    ::grpc::ClientContext *context,
    const ::viam::common::v1::DoCommandRequest &request,
    ::grpc::CompletionQueue *cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<
      ::viam::common::v1::DoCommandResponse,
      ::viam::common::v1::DoCommandRequest, ::grpc::protobuf::MessageLite,
      ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_DoCommand_,
                                     context, request);
}

MockGenericClient::MockGenericClient(std::string name) : GenericClient(name) {
  stub_ = std::make_unique<MockGenericStub>();
}
