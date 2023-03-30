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
    MockGenericStub();

    ::grpc::Status DoCommand(::grpc::ClientContext* context,
                             const ::viam::common::v1::DoCommandRequest& request,
                             ::viam::common::v1::DoCommandResponse* response) override;

   private:
    std::shared_ptr<GenericServer> server;
    std::shared_ptr<::grpc::ChannelInterface> channel_;
    ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>* AsyncDoCommandRaw(
        ::grpc::ClientContext* context,
        const ::viam::common::v1::DoCommandRequest& request,
        ::grpc::CompletionQueue* cq) override {
        throw std::runtime_error("Unimplemented");
    };

    ::grpc::ClientAsyncResponseReader<::viam::common::v1::DoCommandResponse>*
    PrepareAsyncDoCommandRaw(::grpc::ClientContext* context,
                             const ::viam::common::v1::DoCommandRequest& request,
                             ::grpc::CompletionQueue* cq) override {
        throw std::runtime_error("Unimplemented");
    };
};

class MockGenericClient : public GenericClient {
   public:
    MockGenericClient(std::string name);
};
