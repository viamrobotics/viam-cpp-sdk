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

#include <xtensor/xadapt.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xchunked_array.hpp>

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
    if (std::tie(l.name, l.description, l.data_type, l.shape, l.associated_files) !=
        std::tie(r.name, r.description, r.data_type, r.shape, r.associated_files)) {
        return false;
    }

    if (!l.extra != !r.extra)
        return false;
    // ACM TODO: extra comparison, waiting on upstream PR 101
    // return *l.extra == *r.extra;
    return true;
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

             // `name`
             "input1",

             // `description`
             "the first input",

             // `data_type`
             MLModelService::tensor_info::data_type::k_float32,

             // `shape`
             {640, 480, -1},

             // `associated_files`
             {{// `name`
               "path/to/file1.1",

               // `description`
               "i1f1",

               // `label_type`
               MLModelService::tensor_info::file::k_label_type_tensor_value},
              {"path/to/file1.2",
               "i1f2",
               MLModelService::tensor_info::file::k_label_type_tensor_axis}},

             // `extra`
             std::make_shared<AttributeMap::element_type>(
                 std::initializer_list<AttributeMap::element_type::value_type>{
                     {"foo", std::make_shared<ProtoType>("bar")}})},

         {"input2",
          "the second input",
          MLModelService::tensor_info::data_type::k_int32,
          {4096, 2160, 3, -1},
          {{"path/to/file2.1", "i2f1", MLModelService::tensor_info::file::k_label_type_tensor_axis},
           {"path/to/file2.2",
            "i2f2",
            MLModelService::tensor_info::file::k_label_type_tensor_value}},
          std::make_shared<AttributeMap::element_type>(
              std::initializer_list<AttributeMap::element_type::value_type>{
                  {"bar", std::make_shared<ProtoType>(false)}})}},

        // `outputs`
        {{

             // `name`
             "output1",

             // `description`
             "the first output",

             // `data_type`
             MLModelService::tensor_info::data_type::k_int32,

             // `shape`
             {-1, -1},

             // `associated_files`
             {{// `name`
               "path/to/output_file1.1",

               // `description`
               "o1f1",

               // `label_type`
               MLModelService::tensor_info::file::k_label_type_tensor_axis},
              {"path/to/output_file1.2",
               "o1f2",
               MLModelService::tensor_info::file::k_label_type_tensor_value}},

             // `extra`
             std::make_shared<AttributeMap::element_type>(
                 std::initializer_list<AttributeMap::element_type::value_type>{
                     {"baz", std::make_shared<ProtoType>()}})},

         {"output2",
          "the second output",
          MLModelService::tensor_info::data_type::k_float32,
          {-1, -1, 4},
          {{"path/to/output_file2.1",
            "o2f1",
            MLModelService::tensor_info::file::k_label_type_tensor_axis},
           {"path/to/output_file2.2",
            "o2f2",
            MLModelService::tensor_info::file::k_label_type_tensor_value}},
          std::make_shared<AttributeMap::element_type>(
              std::initializer_list<AttributeMap::element_type::value_type>{
                  {"quux", std::make_shared<ProtoType>(3.14)}})}},
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
    mlmodel_server.resource_manager()->add(k_service_name, std::move(mock));

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
    client_server_test(mock, [](auto& client) { BOOST_TEST(test_metadata == client.metadata()); });
}

// XXX ACM TODO
#if 0
BOOST_AUTO_TEST_CASE(mock_infer_grpc_roundtrip) {
    auto mock = std::make_shared<MockMLModelService>();
    client_server_test(mock, [](auto& client) {
        MLModelService::named_tensor_views request;
        auto response = client.infer(request);
        BOOST_TEST(true);
    });
}
#endif
BOOST_AUTO_TEST_SUITE_END()

// This test suite is to validate that we can use xtensor for all of
// the tensor data shuttling we need in order to provide a nice
// presentation layer over the fragemented buffers that proto is going
// to give us for inference data / results. We want to be able to do
// this zero copy, where we just use xtensor to synthesize a properly
// shaped view over the data. We will use the input and output shapes
// from the examples in the scope document.
//
// XXX ACM TODO: Don't need to test chunked.
BOOST_AUTO_TEST_SUITE(xtensor_experiment)

// Test based on getting two 800*600 input images.
BOOST_AUTO_TEST_CASE(xtensor_experiment_mlmodel_scope_detector_input_image) {
    // Pretend proto arrays for two 800/600 8-bit color images that we imagine
    // we want to fuse in to a single input tensor.
    auto image_all_zero = std::vector<uint8_t>(800 * 600, 0);
    auto image_all_ones = std::vector<uint8_t>(800 * 600, 1);

    BOOST_TEST(image_all_zero[0] == 0);
    BOOST_TEST(image_all_ones[0] == 1);

    const std::vector<std::size_t> shape{800, 600};

    // Adopt the data via adapt without taking ownership,
    // this is zero copy.
    auto xtia0 = xt::adapt(image_all_zero.data(), image_all_zero.size(), xt::no_ownership(), shape);

    auto xtia1 = xt::adapt(image_all_ones.data(), image_all_ones.size(), xt::no_ownership(), shape);

    // Create vectors to describe the shape of the tensor and the
    // chunks.
    std::vector<std::size_t> cshape{2, 800, 600};
    std::vector<std::size_t> chunked_shape{1, 800, 600};

    // Make a vector that holds the views on the linear buffers. This
    // is our only allocation.
    std::vector<decltype(xtia0)> xtias_holder{std::move(xtia0), std::move(xtia1)};

    // Adopt the vector with adapt without an ownership transfer.
    auto xtias_adapter = xt::adapt(
        xtias_holder.data(), xtias_holder.size(), xt::no_ownership(), std::vector<std::size_t>{2});

    // Create a chunked array over the vector of chunks. We now have our final view.
    xt::xchunked_array<decltype(xtias_adapter)> xtias(
        std::move(xtias_adapter), cshape, chunked_shape);

    // Validate that dereferencing through the underlying arrays and
    // dereferencing through the chunked array gets the same values.
    BOOST_TEST(image_all_zero[0] == xtias(0, 0, 0));
    BOOST_TEST(image_all_ones[0] == xtias(1, 0, 0));

    // Validate that obtaining references through the underlying
    // arrays and dereferencing through the chunked array gets the
    // same objects..
    BOOST_TEST(&image_all_zero[0] == &xtias(0, 0, 0));
    BOOST_TEST(&image_all_ones[0] == &xtias(1, 0, 0));

    // Mutate the data via the underlying vectors
    image_all_zero[0] = 42;
    image_all_ones[0] = 24;

    // Validate that the mutation is visible when observing through the
    // chunked arary.
    BOOST_TEST(xtias(0, 0, 0) == 42);
    BOOST_TEST(xtias(1, 0, 0) == 24);

    // Mutate the data through the chunked array.
    xtias(0, 0, 0) -= 1;
    xtias(1, 0, 0) -= 1;

    // Validate that the mutations are visible when observing through
    // the underlying buffers.
    BOOST_TEST(image_all_zero[0] == 41);
    BOOST_TEST(image_all_ones[0] == 23);
}

BOOST_AUTO_TEST_CASE(xtensor_experiment_flatten_three_dimensions) {
    // Create a 10x10x10 cube of doubles.
    std::size_t depth = 10;
    std::vector<double> flat(depth * depth * depth, 0);
    std::iota(begin(flat), end(flat), 0.0);
    auto flat_iter = std::begin(flat);

    // Turn that into a vector of vector of vector. This will give us
    // depth^2 memory regions containing our real data.
    std::vector<std::vector<std::vector<double>>> data;
    data.assign(depth, {});
    for (std::size_t i = 0; i < depth; ++i) {
        auto& icurrent = data[i];
        icurrent.assign(depth, {});
        for (std::size_t j = 0; j < depth; j++) {
            auto& jcurrent = icurrent[j];
            jcurrent.assign(flat_iter, flat_iter + depth);
            flat_iter += depth;
        }
    }
    // BOOST_TEST(flat_iter == std::end(flat));
    BOOST_TEST(data[0][0][0] == 0.0);
    BOOST_TEST(data[depth - 1][depth - 1][depth - 1] == flat.back());

    // Create a linear 100 element array of xarray's viewing over the
    // inner contiguous float arrays of `data`.
    //
    // XXX ACM TODO: These shape vectors need dynamic storage. Can we avoid that?
    using base_type = decltype(xt::adapt(
        data[0][0].data(), depth, xt::no_ownership(), std::vector<std::size_t>{depth}));
    std::vector<base_type> chunk_storage;
    chunk_storage.reserve(depth * depth);

    for (std::size_t i = 0; i < depth; ++i) {
        auto& icurrent = data[i];
        for (std::size_t j = 0; j < depth; j++) {
            auto& jcurrent = icurrent[j];
            chunk_storage.emplace_back(xt::adapt(jcurrent.data(),
                                                 jcurrent.size(),
                                                 xt::no_ownership(),
                                                 std::vector<std::size_t>{jcurrent.size()}));
        }
    }

    // Adapt the linear array so we can injest it into a chunked_array.
    auto chunk_adapter = xt::adapt(chunk_storage.data(),
                                   chunk_storage.size(),
                                   xt::no_ownership(),
                                   std::vector<size_t>{chunk_storage.size()});

    // Construct a chunked_array over the adapted linear array. This now looks
    // like a 10x10x10 array over the 100 1x10 chunks.
    std::vector<std::size_t> chunked_shape{depth, depth, depth};
    std::vector<std::size_t> chunk_shape{1, 1, depth};
    const xt::xchunked_array<decltype(chunk_adapter)> tensor_view(
        std::move(chunk_adapter), std::move(chunked_shape), std::move(chunk_shape));

    // Validate that we are index for index and reference for reference between
    // the memory held in `data` and the memory viewed by `tensor_view`.
    for (std::size_t i = 0; i < depth; ++i) {
        for (std::size_t j = 0; j < depth; j++) {
            for (std::size_t k = 0; k < depth; k++) {
                BOOST_TEST(tensor_view(i, j, k) == data[i][j][k]);
                BOOST_TEST(&tensor_view(i, j, k) == &data[i][j][k]);
            }
        }
    }

    // Test that the result of dereferencing the beginning of a chunk gets us a raw pointer
    // to double so that we know that we can get bulk rather than per-element copies in calls
    // like the `insert` in the above below.
    BOOST_TEST(std::is_pointer<decltype((*tensor_view.chunk_begin()).begin())>::value);
    BOOST_TEST(
        (std::is_same<decltype((*tensor_view.chunk_begin()).begin()), const double*>::value));

    // Chunkwise copy the data back into a flattened array and ensure
    // we arrive back at something with the same contents as `flat`.
    std::vector<double> flattened;
    for (auto ci = tensor_view.chunk_begin(); ci != tensor_view.chunk_end(); ++ci) {
        flattened.insert(flattened.end(), (*ci).begin(), (*ci).end());
    }
    BOOST_TEST(flat == flattened);
}

// Test based on getting two sets of 25 bounding boxes (one per input
// image), 25 being arbitrary, represented as float 32.
BOOST_AUTO_TEST_CASE(xtensor_experiment_mlmodel_scope_detector_output_detection_boxes) {
    // Pretend that the model gives us back a linear buffer to represent the tensor

    constexpr std::array<size_t, 3> dimensions{2, 25, 4};
    const std::vector<std::size_t> detection_results_shape{dimensions.begin(), dimensions.end()};

    const std::size_t k_detection_results_buffer_size =
        std::accumulate(begin(detection_results_shape),
                        end(detection_results_shape),
                        1,
                        std::multiplies<std::size_t>());

    const std::vector<float> detection_results_buffer = [&] {
        std::vector<float> temp(k_detection_results_buffer_size, 0);
        std::iota(std::begin(temp), std::end(temp), float{0});
        return temp;
    }();

    BOOST_TEST(detection_results_buffer.front() == 0);
    BOOST_TEST(detection_results_buffer.back() == k_detection_results_buffer_size - 1);

    // Shape the buffer as a tensor and validate that we find the right things at the right indexes.
    //
    // XXX ACM TODO: Things break when I declare this as `const auto`. Why?
    auto detection_results = xt::adapt(detection_results_buffer.data(),
                                       detection_results_buffer.size(),
                                       xt::no_ownership(),
                                       detection_results_shape);

    BOOST_TEST(detection_results(0, 0, 0) == 0);

    BOOST_TEST(
        detection_results(0, detection_results_shape[1] - 1, detection_results_shape[2] - 1) ==
        k_detection_results_buffer_size / 2 - 1);

    BOOST_TEST(detection_results(detection_results_shape[0] - 1,
                                 detection_results_shape[1] - 1,
                                 detection_results_shape[2] - 1) ==
               k_detection_results_buffer_size - 1);

    BOOST_TEST(&detection_results(0, 0, 0) == &detection_results_buffer[0]);
    BOOST_TEST(&detection_results(detection_results_shape[0] - 1,
                                  detection_results_shape[1] - 1,
                                  detection_results_shape[2] - 1) ==
               &detection_results_buffer.back());

    // Validate that we can view this as an `xchunked_array` over a
    // single element vector. This makes it easier to use the same
    // data types for input and output in our `infer` API.
    std::vector<decltype(detection_results)> chunk_storage{detection_results};
    auto chunk_adapter = xt::adapt(chunk_storage.data(),
                                   chunk_storage.size(),
                                   xt::no_ownership(),
                                   std::vector<size_t>{chunk_storage.size()});

    const xt::xchunked_array<decltype(chunk_adapter)> detection_results_chunked(
        std::move(chunk_adapter), detection_results_shape, detection_results_shape);

    BOOST_TEST(detection_results == detection_results_chunked);

    BOOST_TEST(&detection_results_chunked(0, 0, 0) == &detection_results_buffer[0]);
    BOOST_TEST(&detection_results_chunked(detection_results_shape[0] - 1,
                                          detection_results_shape[1] - 1,
                                          detection_results_shape[2] - 1) ==
               &detection_results_buffer.back());

    // XXX ACM TODO: Validate that we can efficiently fragement to 50 newly
    // 4 element vectors with 50 copies, as we would need to
    // do to push it back as a proto `struct`.
    std::array<std::vector<float>, dimensions[0] * dimensions[1]> storage;
    for (auto& storage_item : storage)
        storage_item = std::vector<float>(dimensions[2], float{0});
    // XXX
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace
