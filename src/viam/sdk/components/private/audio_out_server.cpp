#include <viam/sdk/components/private/audio_out_server.hpp>

#include <viam/sdk/common/audio.hpp>
#include <viam/sdk/common/private/service_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/spatialmath/geometry.hpp>

namespace viam {
namespace sdk {
namespace impl {

AudioOutServer::AudioOutServer(std::shared_ptr<ResourceManager> manager)
    : ResourceServer(std::move(manager)) {}

::grpc::Status AudioOutServer::Play(::grpc::ServerContext*,
                                    const ::viam::component::audioout::v1::PlayRequest* request,
                                    ::viam::component::audioout::v1::PlayResponse*) noexcept {
    return make_service_helper<AudioOut>(
        "AudioOutServer::Play", this, request)([&](auto& helper, auto& audio_out) {
        // Convert audio_data from string to std::vector<uint8_t>
        std::vector<uint8_t> audio_data;
        const std::string& audio_data_str = request->audio_data();
        audio_data.assign(audio_data_str.c_str(), audio_data_str.c_str() + audio_data_str.size());

        boost::optional<audio_info> info;
        if (request->has_audio_info()) {
            info.emplace(audio_info{
            request->audio_info().codec(),
            request->audio_info().sample_rate_hz(),
            request->audio_info().num_channels()
    });
        }
        audio_out->play(audio_data, info, helper.getExtra());
    });
}

::grpc::Status AudioOutServer::DoCommand(::grpc::ServerContext*,
                                         const ::viam::common::v1::DoCommandRequest* request,
                                         ::viam::common::v1::DoCommandResponse* response) noexcept {
    return make_service_helper<AudioOut>(
        "AudioOutServer::DoCommand", this, request)([&](auto&, auto& audio_out) {
        const ProtoStruct result = audio_out->do_command(from_proto(request->command()));
        *response->mutable_result() = to_proto(result);
    });
}

::grpc::Status AudioOutServer::GetGeometries(
    ::grpc::ServerContext*,
    const ::viam::common::v1::GetGeometriesRequest* request,
    ::viam::common::v1::GetGeometriesResponse* response) noexcept {
    return make_service_helper<AudioOut>(
        "AudioOutServer::GetGeometries", this, request)([&](auto& helper, auto& audio_out) {
        const std::vector<GeometryConfig> geometries = audio_out->get_geometries(helper.getExtra());
        for (const auto& geometry : geometries) {
            *response->mutable_geometries()->Add() = to_proto(geometry);
        }
    });
}

::grpc::Status AudioOutServer::GetProperties(
    ::grpc::ServerContext*,
    const ::viam::common::v1::GetPropertiesRequest* request,
    ::viam::common::v1::GetPropertiesResponse* response) noexcept {
    return make_service_helper<AudioOut>(
        "AudioOutServer::GetProperties", this, request)([&](auto& helper, auto& audio_out) {
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
