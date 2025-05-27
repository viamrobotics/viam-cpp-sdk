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

#define BOOST_TEST_MODULE test module test_mlmodel
#include <viam/sdk/services/mlmodel.hpp>

#include <memory>
#include <tuple>
#include <unordered_map>

#include <boost/test/included/unit_test.hpp>
#include <boost/variant/get.hpp>

#include <viam/sdk/tests/mocks/mlmodel_mocks.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdk {

bool operator==(const MLModelService::tensor_info::file& l,
                const MLModelService::tensor_info::file& r) {
    return std::tie(l.name, l.description, l.label_type) ==
           std::tie(r.name, r.description, r.label_type);
}

bool operator==(const struct MLModelService::tensor_info& l,
                const struct MLModelService::tensor_info& r) {
    return std::tie(l.name, l.description, l.data_type, l.extra, l.shape, l.associated_files) ==
           std::tie(r.name, r.description, r.data_type, r.extra, r.shape, r.associated_files);
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
         MLModelService::tensor_info::data_types::k_float32,

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
         ProtoStruct{{"foo", ProtoValue{"bar"}}}},

     {"input2",
      "the second input",
      MLModelService::tensor_info::data_types::k_int32,
      {4096, 2160, 3, -1},
      {{"path/to/file2.1", "i2f1", MLModelService::tensor_info::file::k_label_type_tensor_axis},
       {"path/to/file2.2",
        "i2f2",
        MLModelService::tensor_info::file::k_label_type_tensor_value}},
      ProtoStruct{{"bar", ProtoValue{false}}}}},

    // `outputs`
    {{

         // `name`
         "output1",

         // `description`
         "the first output",

         // `data_type`
         MLModelService::tensor_info::data_types::k_int32,

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
         ProtoStruct{{"baz", ProtoValue{}}}},

     {"output2",
      "the second output",
      MLModelService::tensor_info::data_types::k_float32,
      {-1, -1, 4},
      {{"path/to/output_file2.1",
        "o2f1",
        MLModelService::tensor_info::file::k_label_type_tensor_axis},
       {"path/to/output_file2.2",
        "o2f2",
        MLModelService::tensor_info::file::k_label_type_tensor_value}},
      ProtoStruct{{"quux", ProtoValue{3.14}}}}},
};

BOOST_AUTO_TEST_SUITE(test_mock_mlmodel)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockMLModelService mlms("mock_mlms");
    auto api = mlms.api();
    auto static_api = API::get<MLModelService>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "mlmodel");
}

BOOST_AUTO_TEST_CASE(mock_construction) {
    const std::string mock_name = "mocky mock";
    MockMLModelService mock_mlms(mock_name);
    BOOST_TEST(mock_mlms.name() == mock_name);
}

BOOST_AUTO_TEST_CASE(mock_metadata_roundtrip) {
    MockMLModelService mock_mlms;
    mock_mlms.set_metadata(test_metadata);
    BOOST_TEST(mock_mlms.metadata({}) == test_metadata);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_mlmodel_client_server)

BOOST_AUTO_TEST_CASE(mock_metadata_grpc_roundtrip) {
    const auto mock = std::make_shared<MockMLModelService>();
    mock->set_metadata(test_metadata);
    client_to_mock_pipeline<MLModelService>(mock, [](auto& client) {
        const auto returned_metadata = client.metadata();
        BOOST_TEST(test_metadata == returned_metadata);
    });
}

BOOST_AUTO_TEST_CASE(mock_infer_grpc_roundtrip) {
    auto mock = std::make_shared<MockMLModelService>();

    mock->set_metadata({"foo",
                        "bar",
                        "baz",
                        // `inputs`
                        {{"input1",
                          "the first input",
                          MLModelService::tensor_info::data_types::k_float32,
                          {32, 32},
                          {},
                          {}},
                         {"input2",
                          "the second input",
                          MLModelService::tensor_info::data_types::k_int32,
                          {16, 16},
                          {},
                          {}}},
                        // `outputs`
                        {{"output1",
                          "the first output",
                          MLModelService::tensor_info::data_types::k_float32,
                          {32, 32},
                          {},
                          {}},
                         {"output2",
                          "the second output",
                          MLModelService::tensor_info::data_types::k_int32,
                          {16, 16},
                          {},
                          {}}}});

    mock->set_infer_handler([](const MLModelService::named_tensor_views& request) {
        BOOST_REQUIRE(request.size() == 2);
        BOOST_REQUIRE(request.count("input1") == 1);
        BOOST_REQUIRE(request.count("input2") == 1);
        auto input1_var = request.find("input1")->second;
        auto input2_var = request.find("input2")->second;
        const auto* const pinput1_view =
            boost::get<MLModelService::tensor_view<float>>(&input1_var);
        BOOST_REQUIRE(pinput1_view);
        const auto* const pinput2_view =
            boost::get<MLModelService::tensor_view<std::int32_t>>(&input2_var);
        BOOST_REQUIRE(pinput2_view);

        const auto& input1_view = *pinput1_view;
        const auto& input2_view = *pinput2_view;

        auto output1_result = xt::eval(input1_view + xt::ones<float>({32, 32}));
        auto output2_result = xt::eval(input2_view + xt::ones<std::int32_t>({16, 16}));
        struct output_holder {
            decltype(output1_result) output1_holder;
            decltype(output2_result) output2_holder;
            MLModelService::named_tensor_views ntvs;
        };

        auto output = std::make_shared<output_holder>();
        output->output1_holder = std::move(output1_result);
        output->output2_holder = std::move(output2_result);
        output->ntvs.emplace(
            "output1",
            MLModelService::make_tensor_view(
                output->output1_holder.data(),
                output->output1_holder.size(),
                {output->output1_holder.shape().begin(), output->output1_holder.shape().end()}));
        output->ntvs.emplace(
            "output2",
            MLModelService::make_tensor_view(
                output->output2_holder.data(),
                output->output2_holder.size(),
                {output->output2_holder.shape().begin(), output->output2_holder.shape().end()}));
        auto* const ntvs = &output->ntvs;
        return std::shared_ptr<MLModelService::named_tensor_views>{std::move(output), ntvs};
    });

    client_to_mock_pipeline<MLModelService>(mock, [](auto& client) {
        MLModelService::named_tensor_views request;

        std::array<float, 1024> input1_data{};
        input1_data[1] = 1.0;
        auto input1_view =
            MLModelService::make_tensor_view(input1_data.data(), input1_data.size(), {32, 32});

        request.emplace("input1", input1_view);

        std::array<std::int32_t, 256> input2_data{};
        input2_data[2] = 2;
        auto input2_view =
            MLModelService::make_tensor_view(input2_data.data(), input2_data.size(), {16, 16});
        request.emplace("input2", input2_view);

        auto response = client.infer(request);

        BOOST_REQUIRE(response->size() == 2);
        BOOST_REQUIRE(response->count("output1") == 1);
        BOOST_REQUIRE(response->count("output2") == 1);

        auto output1_var = response->find("output1")->second;
        auto output2_var = response->find("output2")->second;

        const auto* const poutput1_view =
            boost::get<MLModelService::tensor_view<float>>(&output1_var);
        BOOST_REQUIRE(poutput1_view);
        const auto* const poutput2_view =
            boost::get<MLModelService::tensor_view<std::int32_t>>(&output2_var);
        BOOST_REQUIRE(poutput2_view);

        const auto& output1_view = *poutput1_view;
        const auto& output2_view = *poutput2_view;
        const bool r1 = (output1_view == input1_view + xt::ones<float>({32, 32}));
        BOOST_TEST(r1);
        const bool r2 = (output2_view == input2_view + xt::ones<std::int32_t>({16, 16}));
        BOOST_TEST(r2);
    });
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_mlmodel_bugfixes)

BOOST_AUTO_TEST_CASE(RSDK_10768) {
    auto mock = std::make_shared<MockMLModelService>();

    mock->set_metadata({"foo",
                        "bar",
                        "baz",
                        // `inputs`
                        {
                            {"input",
                             "the input",
                             MLModelService::tensor_info::data_types::k_float32,
                             {1},
                             {},
                             {}},
                        },
                        // no `outputs`
                        {}});

    mock->set_infer_handler([](const MLModelService::named_tensor_views& request) {
        BOOST_REQUIRE(request.size() == 1);
        BOOST_REQUIRE(request.count("input") == 1);
        return std::make_shared<MLModelService::named_tensor_views>();
    });

    client_to_mock_pipeline<MLModelService>(mock, [&mock](auto& client) {
        MLModelService::named_tensor_views request;

        std::array<float, 1> input_data{};
        input_data[0] = 1.0;
        auto input_view =
            MLModelService::make_tensor_view(input_data.data(), input_data.size(), {1});

        auto mismatched_name = mock->metadata({}).inputs[0].name + "_mismatched";
        request.emplace(std::move(mismatched_name), std::move(input_view));
        auto response = client.infer(request);
        BOOST_TEST(response->size() == 0);
    });
}

BOOST_AUTO_TEST_SUITE_END()

// This test suite is to validate that we can use xtensor for all of
// the tensor data shuttling we need.
BOOST_AUTO_TEST_SUITE(xtensor_experiment)

// Test based on getting two 800*600 input images.
BOOST_AUTO_TEST_CASE(xtensor_experiment_mlmodel_scope_detector_input_image) {
    // Pretend proto arrays for two 800/600 8-bit color images that we imagine
    // we want to fuse in to a single input tensor.
    auto image_all_zero = std::vector<uint8_t>(800 * 600, 0);
    BOOST_TEST(image_all_zero[0] == 0);

    const std::vector<std::size_t> shape{800, 600};

    // Adopt the data via adapt without taking ownership,
    // this is zero copy.
    auto xtia0 = xt::adapt(image_all_zero.data(), image_all_zero.size(), xt::no_ownership(), shape);

    // Validate that dereferencing through the underlying arrays and
    // dereferencing through the chunked array gets the same values.
    BOOST_TEST(image_all_zero[0] == xtia0(0, 0));

    // Validate that obtaining references through the underlying
    // arrays and dereferencing through the chunked array gets the
    // same objects..
    BOOST_TEST(&image_all_zero[0] == &xtia0(0, 0));

    // Mutate the data via the underlying vectors
    image_all_zero[0] = 42;

    // Validate that the mutation is visible when observing through the
    // chunked arary.
    BOOST_TEST(xtia0(0, 0) == 42);

    // Mutate the data through the chunked array.
    xtia0(0, 0) -= 1;

    // Validate that the mutations are visible when observing through
    // the underlying buffers.
    BOOST_TEST(image_all_zero[0] == 41);
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

    // Shape the buffer as a tensor and validate that we find the right things at the right
    // indexes.
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
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(proto)

// TODO: This might be a generally needed facility. Maybe move it to MLModelService.
template <typename T>
struct data_type_for;

template <>
struct data_type_for<std::int8_t> {
    static constexpr auto value = MLModelService::tensor_info::data_types::k_int8;
};

template <>
struct data_type_for<std::uint8_t> {
    static constexpr auto value = MLModelService::tensor_info::data_types::k_uint8;
};

template <>
struct data_type_for<std::int16_t> {
    static constexpr auto value = MLModelService::tensor_info::data_types::k_int16;
};

template <>
struct data_type_for<std::uint16_t> {
    static constexpr auto value = MLModelService::tensor_info::data_types::k_uint16;
};

template <>
struct data_type_for<std::int32_t> {
    static constexpr auto value = MLModelService::tensor_info::data_types::k_int32;
};

template <>
struct data_type_for<std::uint32_t> {
    static constexpr auto value = MLModelService::tensor_info::data_types::k_uint32;
};

template <>
struct data_type_for<std::int64_t> {
    static constexpr auto value = MLModelService::tensor_info::data_types::k_int64;
};

template <>
struct data_type_for<std::uint64_t> {
    static constexpr auto value = MLModelService::tensor_info::data_types::k_uint64;
};

template <>
struct data_type_for<float> {
    static constexpr auto value = MLModelService::tensor_info::data_types::k_float32;
};

template <>
struct data_type_for<double> {
    static constexpr auto value = MLModelService::tensor_info::data_types::k_float64;
};

// A wrapper around a T which will not exhibit UB on signed overflow -
// useful with std::iota with narrow types.
template <typename T>
struct safe_increment {
    safe_increment& operator++() {
        if (val < std::numeric_limits<T>::max())
            ++val;
        else
            val = std::numeric_limits<T>::min();
        return *this;
    }

    operator T() const {
        return val;
    }

    T val;
};

BOOST_AUTO_TEST_CASE_TEMPLATE(rt_scalar, T, MLModelService::base_types) {
    auto mock = std::make_shared<MockMLModelService>();
    mock->set_infer_handler([&](const MLModelService::named_tensor_views& request) {
        return std::make_shared<MLModelService::named_tensor_views>(request);
    });

    const std::array<T, 1> data = {T{42}};
    const auto scalar_tv =
        MLModelService::make_tensor_view(data.data(), data.size(), {data.size()});

    const auto scalar_tv_type = MLModelService::tensor_info::tensor_views_to_data_type(scalar_tv);

    mock->set_metadata({"",
                        "",
                        "",
                        {{"x", "", scalar_tv_type, {1}, {}, {}}},
                        {{"x", "", scalar_tv_type, {1}, {}, {}}}});

    client_to_mock_pipeline<MLModelService>(mock, [&](auto& client) {
        MLModelService::named_tensor_views request;
        request.emplace("x", scalar_tv);
        BOOST_REQUIRE(request.size() == 1);
        BOOST_REQUIRE(request.count("x") == 1);
        const auto response = client.infer(request);
        BOOST_REQUIRE(response->size() == 1);
        BOOST_REQUIRE(response->count("x") == 1);
        const auto& response_x = response->find("x")->second;
        const auto* const response_x_t = boost::get<MLModelService::tensor_view<T>>(&response_x);
        BOOST_REQUIRE(response_x_t);
        BOOST_TEST(scalar_tv == *response_x_t);
    });
}

BOOST_AUTO_TEST_CASE_TEMPLATE(rt_tensor_shapes, T, MLModelService::base_types) {
    using base_type = T;

    constexpr size_t data_size = 4096;
    const std::vector<typename MLModelService::tensor_view<base_type>::shape_type> shapes = {
        {4096},
        {64, 64},
        {16, 16, 16},
        {8, 8, 8, 8},

        {1, 4096},
        {4096, 1},

        {1, 1, 4096},
        {1, 4096, 1},
        {4096, 1, 1},

        {1, 64, 64},
        {64, 1, 64},
        {64, 64, 1},
    };

    auto mock = std::make_shared<MockMLModelService>();
    mock->set_infer_handler([&](const MLModelService::named_tensor_views& request) {
        return std::make_shared<MLModelService::named_tensor_views>(request);
    });

    for (const auto& shape : shapes) {
        BOOST_TEST_REQUIRE(
            std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<std::size_t>()) ==
            data_size);

        std::array<base_type, data_size> data{};
        std::iota(data.begin(), data.end(), safe_increment<T>{base_type{0}});
        const auto array_tv = MLModelService::make_tensor_view(data.data(), data.size(), shape);
        const auto array_tv_type = MLModelService::tensor_info::tensor_views_to_data_type(array_tv);
        const auto array_tv_shape = std::vector<int>{shape.begin(), shape.end()};

        mock->set_metadata({"",
                            "",
                            "",
                            {{"x", "", array_tv_type, array_tv_shape, {}, {}}},
                            {{"x", "", array_tv_type, array_tv_shape, {}, {}}}});

        client_to_mock_pipeline<MLModelService>(mock, [&](auto& client) {
            MLModelService::named_tensor_views request;
            request.emplace("x", array_tv);
            BOOST_REQUIRE(request.size() == 1);
            BOOST_REQUIRE(request.count("x") == 1);
            const auto response = client.infer(request);
            BOOST_REQUIRE(response->size() == 1);
            BOOST_REQUIRE(response->count("x") == 1);
            const auto& response_x = response->find("x")->second;
            const auto* const response_x_t =
                boost::get<MLModelService::tensor_view<T>>(&response_x);
            BOOST_REQUIRE(response_x_t);
            BOOST_TEST(array_tv == *response_x_t);
        });
    }
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace
