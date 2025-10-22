#define BOOST_TEST_MODULE test module test_audio_out

#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/common/audio.hpp>
#include <viam/sdk/components/audio_out.hpp>
#include <viam/sdk/tests/mocks/mock_audio_out.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {

using namespace audioout;
using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_audio_out)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockAudioOut audio_out("mock_audio_out");
    auto api = audio_out.api();
    auto static_api = API::get<AudioOut>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "audio_out");
}


BOOST_AUTO_TEST_CASE(test_play) {
    std::shared_ptr<MockAudioOut> mock = MockAudioOut::get_mock_audio_out();
    client_to_mock_pipeline<AudioOut>(mock, [mock](AudioOut& client) {
        std::vector<std::uint8_t> audio_data;
        audio_data.push_back(1);
        audio_data.push_back(2);
        audio_data.push_back(3);
        audio_data.push_back(4);

        auto info = std::make_shared<audio_info>();
        info->codec = audio_codecs::PCM_16;
        info->sample_rate_hz = 44100;
        info->num_channels = 1;

        client.play(audio_data, info, {});
        BOOST_CHECK(mock->last_played_audio_ == audio_data);
        BOOST_CHECK(mock->last_played_audio_info_ != nullptr);
        BOOST_CHECK(*mock->last_played_audio_info_ == *info);

    });
}

BOOST_AUTO_TEST_CASE(test_play_no_audio_info) {
    std::shared_ptr<MockAudioOut> mock = MockAudioOut::get_mock_audio_out();
    client_to_mock_pipeline<AudioOut>(mock, [mock](AudioOut& client) {
        std::vector<std::uint8_t> audio_data;
        audio_data.push_back(4);
        audio_data.push_back(2);
        audio_data.push_back(4);
        audio_data.push_back(4);

        client.play(audio_data, nullptr, {});
        BOOST_CHECK(mock->last_played_audio_ == audio_data);
        BOOST_CHECK(mock->last_played_audio_info_ == nullptr);

    });
}

BOOST_AUTO_TEST_CASE(test_get_properties) {
    std::shared_ptr<MockAudioOut> mock = MockAudioOut::get_mock_audio_out();
    client_to_mock_pipeline<AudioOut>(mock, [](AudioOut& client) {
        audio_properties expected = fake_properties();

        audio_properties result = client.get_properties({});

        BOOST_CHECK(result == expected);
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockAudioOut> mock = MockAudioOut::get_mock_audio_out();
    client_to_mock_pipeline<AudioOut>(mock, [](AudioOut& client) {
        ProtoStruct expected = fake_map();

        ProtoStruct command = fake_map();
        ProtoStruct result_map = client.do_command(command);

        BOOST_CHECK(result_map.at("test") == expected.at("test"));
    });
}

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    std::shared_ptr<MockAudioOut> mock = MockAudioOut::get_mock_audio_out();
    client_to_mock_pipeline<AudioOut>(mock, [](AudioOut& client) {
        std::vector<GeometryConfig> expected_geometries = fake_geometries();

        std::vector<GeometryConfig> result_geometries = client.get_geometries({});

        BOOST_CHECK(result_geometries == expected_geometries);
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
