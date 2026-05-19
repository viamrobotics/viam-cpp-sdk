// End-to-end test for AudioOut::play_stream against a running rdk server.
//
// Setup:
//   cd /Users/oliviamiller/rdk && go run ./web/cmd/server -config audioout.json
//
// Build (once the CMake project is configurable on your machine):
//   cmake --build build --target audioout_test_playstream
//   ./build/.../audioout_test_playstream

#include <cstdint>
#include <iostream>
#include <vector>

#include <boost/optional.hpp>

#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/audio_out.hpp>
#include <viam/sdk/robot/client.hpp>

using namespace viam::sdk;

int main() {
    auto inst = std::make_shared<Instance>();
    auto client = RobotClient::at_address("localhost:8080", {/*insecure*/ true});

    auto audio_out = client->resource_by_name<AudioOut>("audio_out1");

    audio_info info;
    info.codec = "pcm16";
    info.sample_rate_hz = 22050;
    info.num_channels = 1;

    // chunk_source produces 10 chunks of 4096 bytes, then signals end-of-stream.
    int produced = 0;
    auto chunk_source = [&produced]() -> boost::optional<std::vector<uint8_t>> {
        if (produced >= 10) {
            return boost::none;
        }
        std::vector<uint8_t> chunk(4096, static_cast<uint8_t>(produced));
        produced++;
        return chunk;
    };

    audio_out->play_stream(info, chunk_source, ProtoStruct{});
    std::cout << "ok: play_stream completed (" << produced << " chunks x 4096 bytes)\n";
    return 0;
}
