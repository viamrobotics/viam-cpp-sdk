#include <viam/sdk/components/private/audio_out_server.hpp>

#include <viam/sdk/common/audio.hpp>
#include <viam/sdk/common/private/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>
#include <viam/sdk/tracing/private/span_guard.hpp>

namespace viam {
namespace sdk {
namespace impl {

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
    ::grpc::ServerReader<::viam::component::audioout::v1::PlayStreamRequest>* reader,
    ::viam::component::audioout::v1::PlayStreamResponse*) noexcept {
    ServerSpanGuard span_guard{context, "AudioOutServer::PlayStream"};

    ::viam::component::audioout::v1::PlayStreamRequest first;
    if (!reader->Read(&first)) {
        return span_guard.commit(::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                                                "PlayStream: stream closed before init message"));
    }
    if (!first.has_init()) {
        return span_guard.commit(::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                                                "PlayStream: first message must be PlayStreamInit"));
    }

    const auto& init = first.init();
    audio_info info;
    if (init.has_audio_info()) {
        info.codec = init.audio_info().codec();
        info.sample_rate_hz = init.audio_info().sample_rate_hz();
        info.num_channels = init.audio_info().num_channels();
    }
    const ProtoStruct extra = from_proto(init.extra());

    std::shared_ptr<AudioOut> audio_out;
    try {
        audio_out = resource_manager()->resource<AudioOut>(init.name());
    } catch (const std::exception& e) {
        return span_guard.commit(::grpc::Status(::grpc::StatusCode::NOT_FOUND, e.what()));
    }
    if (!audio_out) {
        return span_guard.commit(::grpc::Status(
            ::grpc::StatusCode::NOT_FOUND, "PlayStream: resource not found: " + init.name()));
    }

    // chunk_source pulls the next chunk off the gRPC stream. Returns boost::none on EOF or
    // cancellation, signaling end-of-stream to the implementation.
    auto chunk_source = [reader, context]() -> boost::optional<std::vector<uint8_t>> {
        ::viam::component::audioout::v1::PlayStreamRequest msg;
        while (reader->Read(&msg)) {
            if (context->IsCancelled()) {
                return boost::none;
            }
            if (msg.payload_case() ==
                ::viam::component::audioout::v1::PlayStreamRequest::kAudioChunk) {
                const std::string& bytes = msg.audio_chunk().audio_data();
                return std::vector<uint8_t>(bytes.begin(), bytes.end());
            }
        }
        return boost::none;
    };

    try {
        audio_out->play_stream(std::move(info), std::move(chunk_source), extra);
    } catch (const std::exception& e) {
        return span_guard.commit(::grpc::Status(::grpc::StatusCode::INTERNAL, e.what()));
    }
    return span_guard.commit(::grpc::Status::OK);
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
