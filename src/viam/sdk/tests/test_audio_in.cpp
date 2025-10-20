#define BOOST_TEST_MODULE test module test_audio_in
#include <viam/sdk/components/audio_in.hpp>
#include <viam/sdk/tests/mocks/mock_audio_in.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/qvm/all.hpp>
#include <boost/test/included/unit_test.hpp>
#include <viam/sdk/tests/test_utils.hpp>

namespace viam {
namespace sdktests {

using namespace audioin;

using namespace viam::sdk;

BOOST_AUTO_TEST_SUITE(test_audio_in)

BOOST_AUTO_TEST_CASE(mock_get_api) {
    const MockAudioIn audio_in("mock_audio_in");
    auto api = audio_in.api();
    auto static_api = API::get<AudioIn>();

    BOOST_CHECK_EQUAL(api, static_api);
    BOOST_CHECK_EQUAL(static_api.resource_subtype(), "audio_in");
}

BOOST_AUTO_TEST_CASE(test_get_audio) {
    std::shared_ptr<MockAudioIn> mock = MockAudioIn::get_mock_audio_in();

    client_to_mock_pipeline<AudioIn>(mock, [&](AudioIn& client) {
        std::vector<AudioIn::AudioChunk> received_chunks;

        // Test streaming for 1 second
        client.get_audio("pcm16",
            [&](AudioIn::AudioChunk&& chunk) -> bool {
                received_chunks.push_back(std::move(chunk));
                return received_chunks.size() < 3; // Stop stream after 3 chunks
            },
            1.0, // 1 second duration
            0    // No previous timestamp
        );

        BOOST_CHECK_EQUAL(received_chunks.size(), 3);

        // Verify chunk properties
        for (const auto& chunk : received_chunks) {
            BOOST_CHECK_GT(chunk.audio_data.size(), 0);
            BOOST_CHECK_EQUAL(chunk.audio_info.codec, "pcm16");
            BOOST_CHECK_EQUAL(chunk.audio_info.sample_rate_hz, 48000);
            BOOST_CHECK_EQUAL(chunk.audio_info.num_channels, 1);
        }
    });
}

BOOST_AUTO_TEST_CASE(test_do_command) {
    std::shared_ptr<MockAudioIn> mock = MockAudioIn::get_mock_audio_in();
    client_to_mock_pipeline<AudioIn>(mock, [](AudioIn& client) {
        ProtoStruct expected = fake_map();

        ProtoStruct command = fake_map();
        ProtoStruct result_map = client.do_command(command);

        BOOST_CHECK(result_map.at("test") == expected.at("test"));
    });
}

BOOST_AUTO_TEST_CASE(test_get_properties) {
    std::shared_ptr<MockAudioIn> mock = MockAudioIn::get_mock_audio_in();
    client_to_mock_pipeline<AudioIn>(mock, [](AudioIn& client) {
        AudioIn::properties props = client.get_properties();
        AudioIn::properties expected = fake_properties();

        BOOST_CHECK(expected == props);
    });
}

BOOST_AUTO_TEST_SUITE_END()


} // namespace sdktests
} //namespace viam
