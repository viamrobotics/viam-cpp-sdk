#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <viam/sdk/components/private/audio_in_server.hpp>

#include <viam/sdk/common/private/service_helper.hpp>

namespace viam {
namespace sdk {
namespace impl {

AudioInServer::AudioInServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status AudioInServer::GetAudio(
    ::grpc::ServerContext* context,
    const ::viam::component::audioin::v1::GetAudioRequest* request,
    ::grpc::ServerWriter<::viam::component::audioin::v1::GetAudioResponse>* writer) noexcept {
    make_service_helper<AudioIn>(
        "AudioInServer::GetAudio", this, request)([&](auto& helper, auto& audio_in) {
        std::string request_id = boost::uuids::to_string(boost::uuids::random_generator()());
        auto writeChunk = [writer, context, request_id](AudioIn::audio_chunk&& chunk) {
            if (context->IsCancelled()) {
                // send bool to tell the resource to stop calling the callback function.
                return false;
            }
            ::viam::component::audioin::v1::GetAudioResponse response;
            auto* audio_chunk = response.mutable_audio();

            // Convert audio_data from std::vector<std::byte> to string
            std::string audio_data_str;
            audio_data_str.reserve(chunk.audio_data.size());
            for (const auto& byte : chunk.audio_data) {
                audio_data_str.push_back(static_cast<char>(byte));
            }
            audio_chunk->set_audio_data(std::move(audio_data_str));

            // Set audio_info fields
            auto* audio_info = audio_chunk->mutable_audio_info();
            audio_info->set_codec(std::move(chunk.audio_info.codec));
            audio_info->set_sample_rate_hz(chunk.audio_info.sample_rate_hz);
            audio_info->set_num_channels(chunk.audio_info.num_channels);

            audio_chunk->set_start_timestamp_nanoseconds(chunk.start_timestamp_ns);
            audio_chunk->set_end_timestamp_nanoseconds(chunk.end_timestamp_ns);
            audio_chunk->set_sequence(chunk.sequence);
            response.set_request_id(request_id);
            writer->Write(response);
            return true;
        };
        audio_in->get_audio(request->codec(),
                            writeChunk,
                            request->duration_seconds(),
                            request->previous_timestamp_nanoseconds(),
                            helper.getExtra());
    });

    return ::grpc::Status();
}

::grpc::Status AudioInServer::DoCommand(::grpc::ServerContext*,
                                        const ::viam::common::v1::DoCommandRequest* request,
                                        ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<AudioIn>(
        "AudioInServer::DoCommand", this, request)([&](auto&, auto& audio_in) {
        const ProtoStruct result = audio_in->do_command(from_proto(request->command()));
        *response->mutable_result() = to_proto(result);
    });
}

::grpc::Status AudioInServer::GetProperties(
    grpc::ServerContext*,
    const viam::common::v1::GetPropertiesRequest* request,
    viam::common::v1::GetPropertiesResponse* response) noexcept {
    return make_service_helper<AudioIn>(
        "AudioInServer::GetProperties", this, request)([&](auto& helper, auto& audio_in) {
        const AudioIn::properties result = audio_in->get_properties(helper.getExtra());
        for (const auto& codec : result.supported_codecs) {
            response->add_supported_codecs(codec);
        }

        response->set_sample_rate_hz(result.sample_rate_hz);
        response->set_num_channels(result.num_channels);
    });
}

::grpc::Status AudioInServer::GetGeometries(
    ::grpc::ServerContext*,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<AudioIn>(
        "AudioInServer::GetGeometries", this, request)([&](auto& helper, auto& audio_in) {
        const std::vector<GeometryConfig> geometries = audio_in->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = to_proto(geometry);
        }
    });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
