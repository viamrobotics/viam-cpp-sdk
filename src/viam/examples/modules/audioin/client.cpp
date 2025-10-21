#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <viam/sdk/common/audio.hpp>
#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/audio_in.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/rpc/dial.hpp>

using namespace viam::sdk;

int main() {
    // Every Viam C++ SDK program must have one and only one Instance object which is created before
    // any other C++ SDK objects and stays alive until all Viam C++ SDK objects are destroyed.
    Instance inst;

    std::string host("xarm-main.aqb785vhl4.viam.cloud");
    DialOptions dial_opts;
    dial_opts.set_entity(std::string("88dcef8e-db7f-47dc-9b0f-eb08fdc5a97d"));

    Credentials credentials("api-key", "eou798fi90d1fytv66c9lntv1ndwxj4g");

    dial_opts.set_credentials(credentials);
    boost::optional<DialOptions> opts(dial_opts);
    Options options(0, opts);

    auto robot = RobotClient::at_address(host, options);

    // // Update these with your robot's connection details
    // const char* uri = "";  // replace with your robot's URI
    // DialOptions dial_options;
    // dial_options.set_allow_insecure_downgrade(true);  // set to false if connecting securely

    // // Uncomment and fill out your credentials details if connecting securely
    // // std::string type = "api-key";
    // // std::string payload = "your-api-key-here";
    // // Credentials credentials(type, payload);
    // // dial_options.set_credentials(credentials);

    // boost::optional<DialOptions> opts(dial_options);
    // std::string address(uri);
    // Options options(1, opts);

    // std::shared_ptr<RobotClient> robot = RobotClient::at_address(address, options);

    // Print resources
    VIAM_SDK_LOG(info) << "Resources:";
    std::vector<Name> resource_names = robot->resource_names();
    for (const Name& resource : resource_names) {
        VIAM_SDK_LOG(info) << "  " << resource;
    }

    // Get the AudioIn component (update with your component name)
    auto audio_in = robot->resource_by_name<AudioIn>("sinewave-audio");
    if (!audio_in) {
        VIAM_SDK_LOG(error) << "could not get 'sinewave-audio' resource from robot";
        return EXIT_FAILURE;
    }

    VIAM_SDK_LOG(info) << "Getting audio properties...";
    properties props = audio_in->get_properties();
    VIAM_SDK_LOG(info) << "Audio properties:";
    VIAM_SDK_LOG(info) << "  sample_rate_hz: " << props.sample_rate_hz;
    VIAM_SDK_LOG(info) << "  num_channels: " << props.num_channels;
    VIAM_SDK_LOG(info) << "  supported_codecs: " << props.supported_codecs.size() << " codecs";

    VIAM_SDK_LOG(info) << "Retrieving 2 seconds of audio...";

    std::vector<uint8_t> all_audio_data;
    int chunk_count = 0;

    // Define chunk handler to collect audio data
    auto chunk_handler = [&](AudioIn::audio_chunk&& chunk) -> bool {
        chunk_count++;
        VIAM_SDK_LOG(info) << "Received chunk " << chunk_count
                           << " - length: " << chunk.audio_data.size()
                           << " bytes";

        for (const auto& byte : chunk.audio_data) {
            all_audio_data.push_back(static_cast<uint8_t>(byte));
        }

        return true;  // Continue receiving chunks
    };

    // Get 2 seconds of audio (with previous_timestamp = 0 to start from now)
    audio_in->get_audio(audio_codecs::PCM_16, chunk_handler, 2.0, 0);

    VIAM_SDK_LOG(info) << "Total audio data received: " << all_audio_data.size() << " bytes";
    VIAM_SDK_LOG(info) << "Total chunks: " << chunk_count;

    std::string filename = "sine_wave_audio.wav";
    try {
        write_wav_file(filename, all_audio_data, audio_codecs::PCM_16, props.sample_rate_hz, props.num_channels);
        VIAM_SDK_LOG(info) << "Audio saved to " << filename;
        VIAM_SDK_LOG(info) << "To play: open " << filename << " (or use any audio player)";
    } catch (const std::exception& e) {
        VIAM_SDK_LOG(error) << "Failed to write WAV file: " << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
