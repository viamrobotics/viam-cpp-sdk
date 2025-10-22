#include <viam/sdk/components/private/audio_out_client.hpp>

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/audioout/v1/audioout.grpc.pb.h>
#include <viam/api/component/audioout/v1/audioout.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/audio_out.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

AudioOutClient::AudioOutClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : AudioOut(std::move(name)),
      stub_(viam::component::audioout::v1::AudioOutService::NewStub(channel)),
      channel_(std::move(channel)) {}

void AudioOutClient::play(std::vector<uint8_t> const& audio_data, std::shared_ptr<audio_info> info, const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Play)
        .with(extra, [&](auto& request) {
            // Convert audio_data from std::vector<uint8_t> to string
            std::string audio_data_str;
            audio_data_str.reserve(audio_data.size());
            for (const auto& byte : audio_data) {
                audio_data_str.push_back(static_cast<char>(byte));
            }
            request.set_audio_data(std::move(audio_data_str));

            if (info) {
                auto* proto_info = request.mutable_audio_info();
                proto_info->set_codec(info->codec);
                proto_info->set_sample_rate_hz(info->sample_rate_hz);
                proto_info->set_num_channels(info->num_channels);
            }
        })
        .invoke([](auto& response) {});
}

audio_properties AudioOutClient::get_properties(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetProperties)
        .with(extra)
        .invoke([](auto& response) {
            // Convert repeated field to vector
            std::vector<std::string> codecs;
            codecs.reserve(response.supported_codecs_size());
            for (const auto& codec : response.supported_codecs()) {
                codecs.push_back(codec);
            }

            return audio_properties{std::move(codecs),
                              response.sample_rate_hz(),
                              response.num_channels()};
        });
}

ProtoStruct AudioOutClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
}

std::vector<GeometryConfig> AudioOutClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
