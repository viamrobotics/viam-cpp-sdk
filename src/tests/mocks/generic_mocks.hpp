#pragma once

#include <common/v1/common.pb.h>
#include <component/camera/v1/camera.grpc.pb.h>
#include <component/camera/v1/camera.pb.h>

#include <components/generic/generic.hpp>
#include <components/generic/server.hpp>

class MockGeneric : public Generic {
   public:
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> do_command(
        std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> command)
        override;

    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> map;
    static std::shared_ptr<MockGeneric> get_mock_generic();
};

class MockGenericStub : public viam::component::generic::v1::GenericService::StubInterface {
   public:
    GenericServer server;

    MockGenericStub();

    ::grpc::Status DoCommand(::grpc::ClientContext* context,
                             const ::viam::common::v1::DoCommandRequest& request,
                             ::viam::common::v1::DoCommandResponse* response) override;
    std::unique_ptr<::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>>
    AsyncDoCommand(::grpc::ClientContext* context,
                   const ::viam::common::v1::DoCommandRequest& request,
                   ::grpc::CompletionQueue* cq);
    std::unique_ptr<::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>>
    PrepareAsyncDoCommand(::grpc::ClientContext* context,
                          const ::viam::common::v1::DoCommandRequest& request,
                          ::grpc::CompletionQueue* cq);

    class async final : public StubInterface::async_interface {
       public:
        void DoCommand(::grpc::ClientContext* context,
                       const ::viam::common::v1::DoCommandRequest* request,
                       ::viam::common::v1::DoCommandResponse* response,
                       std::function<void(::grpc::Status)>) override;

        void DoCommand(::grpc::ClientContext* context,
                       const ::viam::common::v1::DoCommandRequest* request,
                       ::viam::common::v1::DoCommandResponse* response,
                       ::grpc::ClientUnaryReactor* reactor) override;

       public:
        friend class Stub;
        explicit async(MockGenericStub* stub) : stub_(stub) {}
        MockGenericStub* stub() {
            return stub_;
        }
        MockGenericStub* stub_;
    };
    class async* async() override {
        return &async_stub_;
    }

   private:
    std::shared_ptr<::grpc::ChannelInterface> channel_;
    class async async_stub_ {
        this
    };
    ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>* AsyncDoCommandRaw(
        ::grpc::ClientContext* context,
        const ::viam::common::v1::DoCommandRequest& request,
        ::grpc::CompletionQueue* cq) override;

    ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>*
    PrepareAsyncDoCommandRaw(::grpc::ClientContext* context,
                             const ::viam::common::v1::DoCommandRequest& request,
                             ::grpc::CompletionQueue* cq) override;

    const ::grpc::internal::RpcServiceMethod::RpcType type =
        ::grpc::internal::RpcServiceMethod::RpcType();
    const ::grpc::internal::RpcMethod rpcmethod_DoCommand_ =
        ::grpc::internal::RpcMethod("name", type);
};

class MockGenericClient : public GenericClient {
   public:
    MockGenericClient(std::string name);
};
