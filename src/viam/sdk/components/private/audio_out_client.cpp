#include <viam/sdk/components/private/audio_out_client.hpp>

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <grpcpp/channel.h>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/audioout/v1/audioout.grpc.pb.h>
#include <viam/api/component/audioout/v1/audioout.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/audio_out.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

AudioOutClient::AudioOutClient(std::string name, const ViamChannel& channel)
    : AudioOut(std::move(name)),
      stub_(viam::component::audioout::v1::AudioOutService::NewStub(
          std::static_pointer_cast<::grpc::ChannelInterface>(channel.channel()))),
      channel_(&channel) {}

void AudioOutClient::play_stream(
    audio_info info,
    std::function<boost::optional<std::vector<uint8_t>>()> chunk_source,
    const ProtoStruct& extra) {
    ::grpc::ClientContext ctx;
    ::viam::component::audioout::v1::PlayStreamResponse response;
    auto writer = stub_->PlayStream(&ctx, &response);

    // First message: init (name + audio_info + extra).
    ::viam::component::audioout::v1::PlayStreamRequest init;
    auto* init_payload = init.mutable_init();
    init_payload->set_name(this->name());
    auto* pb_info = init_payload->mutable_audio_info();
    pb_info->set_codec(info.codec);
    pb_info->set_sample_rate_hz(info.sample_rate_hz);
    pb_info->set_num_channels(info.num_channels);
    *init_payload->mutable_extra() = to_proto(extra);
    if (!writer->Write(init)) {
        const auto status = writer->Finish();
        throw GRPCException(&status);
    }

    // Pull chunks from the source and forward each as a PlayStreamChunk message.
    while (auto chunk = chunk_source()) {
        ::viam::component::audioout::v1::PlayStreamRequest msg;
        auto* chunk_msg = msg.mutable_audio_chunk();
        chunk_msg->set_audio_data(std::string(chunk->begin(), chunk->end()));
        if (!writer->Write(msg)) {
            // Stream broken on server side; surface the error from Finish().
            break;
        }
    }

    writer->WritesDone();
    const auto status = writer->Finish();
    if (!status.ok()) {
        throw GRPCException(&status);
    }
}

void AudioOutClient::play(std::vector<uint8_t> const& audio_data,
                          boost::optional<audio_info> info,
                          const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::Play)
        .with(extra,
              [&](auto& request) {
                  // Convert audio_data from std::vector<uint8_t> to string
                  std::string audio_data_str(audio_data.begin(), audio_data.end());
                  request.set_audio_data(std::move(audio_data_str));

                  if (info) {
                      ::viam::common::v1::AudioInfo* proto_info = request.mutable_audio_info();
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
            // Convert proto repeated field to vector
            std::vector<std::string> codecs;
            codecs.reserve(response.supported_codecs_size());
            for (const auto& codec : response.supported_codecs()) {
                codecs.push_back(codec);
            }

            return audio_properties{
                std::move(codecs), response.sample_rate_hz(), response.num_channels()};
        });
}

ProtoStruct AudioOutClient::do_command(const ProtoStruct& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)
        .with([&](auto& request) { *request.mutable_command() = to_proto(command); })
        .invoke([](auto& response) { return from_proto(response.result()); });
}

ProtoStruct AudioOutClient::get_status() {
    return make_client_helper(this, *stub_, &StubType::GetStatus).invoke([](auto& response) {
        return from_proto(response.result());
    });
}

std::vector<GeometryConfig> AudioOutClient::get_geometries(const ProtoStruct& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)
        .with(extra)
        .invoke([](auto& response) { return from_proto(response); });
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
