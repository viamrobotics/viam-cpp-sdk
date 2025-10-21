#include <viam/sdk/components/private/audio_in_client.hpp>

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/audioin/v1/audioin.grpc.pb.h>
#include <viam/api/component/audioin/v1/audioin.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/linear_algebra.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/base.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

AudioInClient::AudioInClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : AudioIn(std::move(name)),
      stub_(viam::component::audioin::v1::AudioInService::NewStub(channel)),
      channel_(std::move(channel)) {}

void AudioInClient::get_audio(std::string const& codec,
                              std::function<bool(audio_chunk&& chunk)> const& chunk_handler,
                              double const& duration_seconds,
                              int64_t const& previous_timestamp,
                              const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetAudio)
        .with(extra,
              [&](auto& request) {
                  request.set_codec(codec);
                  request.set_duration_seconds(duration_seconds);
                  request.set_previous_timestamp_nanoseconds(previous_timestamp);
              })
        .invoke_stream([&](auto& response) {
            // Create audio_chunk struct from proto response
            audio_chunk chunk;

            // Convert audio_data from string to std::vector<std::byte>
            const std::string& audio_data_str = response.audio().audio_data();
            chunk.audio_data.reserve(audio_data_str.size());
            for (char c : audio_data_str) {
                chunk.audio_data.push_back(static_cast<std::byte>(c));
            }

            chunk.sequence = response.audio().sequence();
            chunk.request_id = response.request_id();

            if (response.audio().has_audio_info()) {
                chunk.audio_info.codec = response.audio().audio_info().codec();
                chunk.audio_info.sample_rate_hz = response.audio().audio_info().sample_rate_hz();
                chunk.audio_info.num_channels = response.audio().audio_info().num_channels();
            }
            return chunk_handler(std::move(chunk));
        });
}

AudioIn::properties AudioInClient::get_properties(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetProperties)
        .with(extra)
        .invoke([](auto& response) {
            // Convert proto repeated field to vector
            std::vector<std::string> codecs;
            codecs.reserve(response.supported_codecs_size());
            for (const auto& codec : response.supported_codecs()) {
                codecs.push_back(codec);
            }

            return properties{
                std::move(codecs), response.sample_rate_hz(), response.num_channels()};
        });
}

ProtoStruct AudioInClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
}

std::vector<GeometryConfig> AudioInClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
