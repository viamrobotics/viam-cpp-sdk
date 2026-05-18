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

AudioOutClientStreamWriter::AudioOutClientStreamWriter(
    std::unique_ptr<::grpc::ClientWriter< ::viam::component::audioout::v1::PlayStreamRequest>> writer,
    audio_info info,
    const ProtoStruct& extra,
    const std::string& name)
    : writer_(std::move(writer)) {
    ::viam::component::audioout::v1::PlayStreamInit init_message;
    init_message.set_name(name);
    ::viam::common::v1::AudioInfo* proto_info = init_message.mutable_audio_info();
    proto_info->set_codec(info.codec);
    proto_info->set_sample_rate_hz(info.sample_rate_hz);
    proto_info->set_num_channels(info.num_channels);
    *init_message.mutable_extra() = to_proto(extra);

    ::viam::component::audioout::v1::PlayStreamRequest init_request;
    *init_request.mutable_init() = init_message;
    writer_->Write(init_request);
}

void AudioOutClientStreamWriter::write(std::vector<uint8_t> const& audio_data) {
    ::viam::component::audioout::v1::PlayStreamChunk chunk_message;
    std::string audio_data_str(audio_data.begin(), audio_data.end());
    chunk_message.set_audio_data(std::move(audio_data_str));

    ::viam::component::audioout::v1::PlayStreamRequest chunk_request;
    *chunk_request.mutable_audio_chunk() = chunk_message;
    writer_->Write(chunk_request);
}

void AudioOutClientStreamWriter::close() {
    writer_->WritesDone();
    writer_->Finish(&response_);
}

std::unique_ptr<AudioOutStream> AudioOutClient::play_stream(audio_info info,
                                                           const ProtoStruct& extra) {
    ::grpc::ClientContext context;
    ::viam::component::audioout::v1::PlayStreamResponse response;
    auto writer = stub_->PlayStream(&context, &response);
    return std::make_unique<AudioOutClientStreamWriter>(std::move(writer), info, extra, name());
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam