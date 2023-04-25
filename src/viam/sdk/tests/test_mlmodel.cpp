// Copyright 2023 Viam Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <tuple>

#include <viam/sdk/services/mlmodel/client.hpp>
#include <viam/sdk/services/mlmodel/mlmodel.hpp>
#include <viam/sdk/services/mlmodel/server.hpp>
#include <viam/sdk/tests/mocks/mlmodel_mocks.hpp>

#define BOOST_TEST_MODULE test module test_mlmodel
#include <boost/test/included/unit_test.hpp>

namespace viam {
namespace sdk {
bool operator==(const MLModelService::tensor_info::file& l,
                const MLModelService::tensor_info::file& r) {
    return std::tie(l.name, l.description, l.label_type) ==
           std::tie(r.name, r.description, r.label_type);
}

bool operator==(const struct MLModelService::tensor_info& l,
                const struct MLModelService::tensor_info& r) {
    return std::tie(l.name, l.description, l.data_type, l.shape, l.associated_files) ==
           std::tie(r.name, r.description, r.data_type, r.shape, r.associated_files);
}

bool operator==(const struct MLModelService::metadata& l,
                const struct MLModelService::metadata& r) {
    return std::tie(l.name, l.type, l.description, l.inputs, l.outputs) ==
           std::tie(r.name, r.type, r.description, r.inputs, r.outputs);
}
}  // namespace sdk
}  // namespace viam

BOOST_TEST_DONT_PRINT_LOG_VALUE(struct viam::sdk::MLModelService::metadata)

namespace {

using namespace ::viam::sdk;
using namespace ::viam::sdktests;

const struct MLModelService::metadata test_metadata {
    // `name`
    "my model",

        // `type`
        "magic",

        // `description`,
        "Convolutional neural network ...",

        // `inputs`
        {{
             "input1",
             "the first input",
             "float32",
             {640, 480, -1},
             {{"path/to/file1.1", "i1f1", MLModelService::tensor_info::file::k_type_tensor_value},
              {"path/to/file1.2", "i1f2", MLModelService::tensor_info::file::k_type_tensor_axis}},
             // TODO `extra` field
         },

         {
             "input2",
             "the second input",
             "int32",
             {4096, 2160, 3, -1},
             {{"path/to/file2.1", "i2f1", MLModelService::tensor_info::file::k_type_tensor_axis},
              {"path/to/file2.2", "i2f2", MLModelService::tensor_info::file::k_type_tensor_value}},
             // TODO `extra` field
         }},

    // `outputs`
    {}
};

BOOST_AUTO_TEST_SUITE(test_mock_mlmodel)

BOOST_AUTO_TEST_CASE(mock_construction) {
    const std::string mock_name = "mocky mock";
    MockMLModelService mock_mlms(mock_name);
    BOOST_TEST(mock_mlms.name() == mock_name);
}

BOOST_AUTO_TEST_CASE(mock_metadata_roundtrip) {
    MockMLModelService mock_mlms;
    mock_mlms.metadata(test_metadata);
    BOOST_TEST(mock_mlms.metadata() == test_metadata);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_mlmodel_client_server)

template <typename F>
void client_server_test(std::shared_ptr<MockMLModelService> mock, F&& f) {
    const auto k_service_name = "mock_mlmodel_service";

    MLModelServiceServer mlmodel_server;
    mlmodel_server.get_sub_svc()->add(
        k_service_name,
        std::move(mock)
    );

    grpc::ServerBuilder builder;
    builder.RegisterService(&mlmodel_server);
    auto server = builder.BuildAndStart();

    MLModelServiceClient client(k_service_name, server->InProcessChannel({}));
    std::forward<F>(f)(client);

    server->Shutdown();
}

BOOST_AUTO_TEST_CASE(mock_metadata_grpc_roundtrip) {
    auto mock = std::make_shared<MockMLModelService>();
    mock->metadata(test_metadata);
    client_server_test(mock, [](auto& client) {
        auto client_metadata = client.metadata();
        BOOST_TEST(test_metadata == client_metadata);
    });
}
BOOST_AUTO_TEST_SUITE_END()

}  // namespace
