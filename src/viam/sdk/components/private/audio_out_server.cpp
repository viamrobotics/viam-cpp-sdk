#include <viam/sdk/components/private/audio_out_server.hpp>

#include <viam/sdk/common/audio.hpp>
#include <viam/sdk/common/private/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {
namespace impl {

// AudioOutServerStreamReader
AudioOutServerStreamReader::AudioOutServerStreamReader(
    ::grpc::ServerReader< ::viam::component::audioout::v1::PlayStreamRequest>* reader)
    : reader_(reader) {}

boost::optional<std::vector<uint8_t>> AudioOutServerStreamReader::read() {
    ::viam::component::audioout::v1::PlayStreamRequest request;
    if (reader_->Read(&request)) {
        if (request.payload_case() == ::viam::component::audioout::v1::PlayStreamRequest::kAudioChunk) {
            return boost::make_optional(string_to_bytes(request.audio_chunk().audio_data()));
        }
    }
    return boost::none;
}

AudioOutServer::AudioOutServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status AudioOutServer::Play(::grpc::ServerContext* context,
                                    const ::viam::component::audioout::v1::PlayRequest* request,
                                    ::viam::component::audioout::v1::PlayResponse*) noexcept {
    return make_service_helper<AudioOut>(
        "AudioOutServer::Play", this, context, request)([&](auto& helper, auto& audio_out) {
        // Convert audio_data from string to std::vector<uint8_t>
        std::vector<uint8_t> audio_data;
        const std::string& audio_data_str = request->audio_data();
        audio_data.assign(audio_data_str.c_str(), audio_data_str.c_str() + audio_data_str.size());

        boost::optional<audio_info> info;
        if (request->has_audio_info()) {
            info.emplace(audio_info{request->audio_info().codec(),
                                    request->audio_info().sample_rate_hz(),
                                    request->audio_info().num_channels()});
        }
        audio_out->play(audio_data, info, helper.getExtra());
    });
}

::grpc::Status AudioOutServer::PlayStream(
    ::grpc::ServerContext* context,
    ::grpc::ServerReader< ::viam::component::audioout::v1::PlayStreamRequest>* reader) {
    return make_service_helper<AudioOut>(
        "AudioOutServer::PlayStream", this, context, reader)(
        [&](auto& helper, auto& audio_out) {
            ::viam::component::audioout::v1::PlayStreamRequest request;
            if (!reader->Read(&request)) {
                throw std::runtime_error("failed to read first PlayStreamRequest");
            }

            if (request.payload_case() !=
                ::viam::component::audioout::v1::PlayStreamRequest::kInit) {
                throw std::invalid_argument(
                    "first message must be PlayStreamInit");
            }

            const auto& init = request.init();
            const auto& audio_info_proto = init.audio_info();
            const auto info = audio_info{audio_info_proto.codec(),
                                         audio_info_proto.sample_rate_hz(),
                                         audio_info_proto.num_channels()};

            std::unique_ptr<AudioOutStreamReader> reader_wrapper =
                std::make_unique<AudioOutServerStreamReader>(reader);

            audio_out->play_stream(std::move(reader_wrapper), info, helper.getExtra());
        });
}

::grpc::Status AudioOutServer::DoCommand(::grpc::ServerContext* context,
                                         const ::viam::common::v1::DoCommandRequest* request,
                                         ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<AudioOut>(
        "AudioOutServer::DoCommand", this, context, request)([&](auto&, auto& audio_out) {
        const ProtoStruct result = audio_out->do_command(from_proto(request->command()));
        *response->mutable_result() = to_proto(result);
    });
}

::grpc::Status AudioOutServer::GetStatus(::grpc::ServerContext* context,
                                         const ::viam::common::v1::GetStatusRequest* request,
                                         ::viam::common::v1::GetStatusResponse* response) noexcept {
    return make_service_helper<AudioOut>(
        "AudioOutServer::GetStatus", this, context, request)([&](auto&, auto& audio_out) {
        const ProtoStruct result = audio_out->get_status();
        *response->mutable_result() = to_proto(result);
    });
}

::grpc::Status AudioOutServer::GetGeometries(
    ::grpc::ServerContext* context,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<AudioOut>("AudioOutServer::GetGeometries", this, context, request)(
        [&](auto& helper, auto& audio_out) {
            const std::vector<GeometryConfig> geometries =
                audio_out->get_geometries(helper.getExtra());
            for (const auto& geometry : geometries) {
                *response->mutable_geometries()->Add() = to_proto(geometry);
            }
        });
}

::grpc::Status AudioOutServer::GetProperties(
    ::grpc::ServerContext* context,
    const ::viam::common::v1::GetPropertiesRequest* request,
    ::viam::common::v1::GetPropertiesResponse* response) noexcept {
    return make_service_helper<AudioOut>("AudioOutServer::GetProperties", this, context, request)(
        [&](auto& helper, auto& audio_out) {
            const audio_properties result = audio_out->get_properties(helper.getExtra());

            // Copy supported_codecs vector to repeated field
            for (const auto& codec : result.supported_codecs) {
                response->add_supported_codecs(codec);
            }

            response->set_sample_rate_hz(result.sample_rate_hz);
            response->set_num_channels(result.num_channels);
        });
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam