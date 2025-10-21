#define BOOST_TEST_MODULE test module test_audio_in
#include <boost/optional/optional_io.hpp>
#include <boost/qvm/all.hpp>
#include <boost/test/included/unit_test.hpp>
#include <viam/sdk/components/audio_in.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/tests/mocks/mock_audio_in.hpp>
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
        std::vector<AudioIn::audio_chunk> received_chunks;

        client.get_audio(
            "pcm16",
            [&](AudioIn::audio_chunk&& chunk) -> bool {
                received_chunks.push_back(std::move(chunk));
                // Stop stream after 3 chunks
                return received_chunks.size() < 3;
            },
            1.0,  // 1 second duration
            0     // No previous timestamp
        );

        BOOST_CHECK_EQUAL(received_chunks.size(), 3);

        // Check that request_id is consistent across all chunks
        BOOST_CHECK(!received_chunks[0].request_id.empty());
        std::string first_request_id = received_chunks[0].request_id;

        for (size_t i = 0; i < received_chunks.size(); ++i) {
            const auto& chunk = received_chunks[i];
            BOOST_CHECK_EQUAL(chunk.audio_data.size(), 1024);
            BOOST_CHECK_EQUAL(chunk.info.codec, "pcm16");
            BOOST_CHECK_EQUAL(chunk.info.sample_rate_hz, 48000);
            BOOST_CHECK_EQUAL(chunk.info.num_channels, 1);
            BOOST_CHECK_EQUAL(chunk.request_id, first_request_id);
            BOOST_CHECK_EQUAL(chunk.sequence, static_cast<int>(i));
            BOOST_CHECK_GE(chunk.start_timestamp_ns, 0);
            BOOST_CHECK_GE(chunk.end_timestamp_ns, 0);
        }
    });
}

BOOST_AUTO_TEST_CASE(test_get_audio_request_ids_differ_across_calls) {
    std::shared_ptr<MockAudioIn> mock = MockAudioIn::get_mock_audio_in();

    client_to_mock_pipeline<AudioIn>(mock, [&](AudioIn& client) {
        std::string first_request_id;
        std::string second_request_id;

        // First call
        client.get_audio(
            "pcm16",
            [&](AudioIn::audio_chunk&& chunk) -> bool {
                first_request_id = chunk.request_id;
                return false;  // Stop after first chunk
            },
            1.0,
            0);

        // Second call
        client.get_audio(
            "pcm16",
            [&](AudioIn::audio_chunk&& chunk) -> bool {
                second_request_id = chunk.request_id;
                return false;  // Stop after first chunk
            },
            1.0,
            0);

        // Request IDs should be different across separate calls
        BOOST_CHECK(!first_request_id.empty());
        BOOST_CHECK(!second_request_id.empty());
        BOOST_CHECK_NE(first_request_id, second_request_id);
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

BOOST_AUTO_TEST_CASE(test_get_geometries) {
    std::shared_ptr<MockAudioIn> mock = MockAudioIn::get_mock_audio_in();
    client_to_mock_pipeline<AudioIn>(mock, [](AudioIn& client) {
        std::vector<GeometryConfig> result_geometries = client.get_geometries();
        std::vector<GeometryConfig> expected_geometries = fake_geometries();

        BOOST_CHECK(result_geometries == expected_geometries);
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
