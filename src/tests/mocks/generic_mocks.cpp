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
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> command) {
    return map;
}

std::shared_ptr<MockGeneric> MockGeneric::get_mock_generic() {
    auto generic = std::make_shared<MockGeneric>();
    generic->map = fake_map();

    return generic;
}

MockGenericStub::MockGenericStub() : server(std::make_shared<GenericServer>()) {
    this->server->get_sub_svc()->add(std::string("generic"), MockGeneric::get_mock_generic());
}

::grpc::Status MockGenericStub::DoCommand(::grpc::ClientContext* context,
                                          const ::viam::common::v1::DoCommandRequest& request,
                                          ::viam::common::v1::DoCommandResponse* response) {
    grpc::ServerContext* ctx;
    return server->DoCommand(ctx, &request, response);
}

MockGenericClient::MockGenericClient(std::string name) : GenericClient(name) {
    stub_ = std::make_unique<MockGenericStub>();
}
