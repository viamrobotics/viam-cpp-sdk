#include <viam/sdk/components/encoder/encoder.hpp>

#include <google/protobuf/descriptor.h>

#include <viam/api/component/encoder/v1/encoder.grpc.pb.h>
#include <viam/api/component/encoder/v1/encoder.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/encoder/client.hpp>
#include <viam/sdk/components/encoder/server.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

EncoderRegistration::EncoderRegistration(
    const google::protobuf::ServiceDescriptor* service_descriptor)
    : ResourceRegistration(service_descriptor){};

std::shared_ptr<ResourceServer> EncoderRegistration::create_resource_server(
    std::shared_ptr<ResourceManager> manager) {
    return std::make_shared<EncoderServer>(manager);
};

std::shared_ptr<Resource> EncoderRegistration::create_rpc_client(
    std::string name, std::shared_ptr<grpc::Channel> chan) {
    return std::make_shared<EncoderClient>(std::move(name), std::move(chan));
};

std::shared_ptr<ResourceRegistration> Encoder::resource_registration() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::encoder::v1::EncoderService::service_full_name());
    if (!sd) {
        throw std::runtime_error("Unable to get service descriptor for the encoder service");
    }
    return std::make_shared<EncoderRegistration>(sd);
}

API Encoder::static_api() {
    return {kRDK, kComponent, "encoder"};
}

API Encoder::dynamic_api() const {
    return static_api();
}

Encoder::position_type Encoder::from_proto(viam::component::encoder::v1::PositionType proto) {
    switch (proto) {
        case viam::component::encoder::v1::POSITION_TYPE_UNSPECIFIED: {
            return Encoder::position_type::unspecified;
        }
        case viam::component::encoder::v1::POSITION_TYPE_ANGLE_DEGREES: {
            return Encoder::position_type::angle_degrees;
        }
        case viam::component::encoder::v1::POSITION_TYPE_TICKS_COUNT: {
            return Encoder::position_type::ticks_count;
        }
        default: {
            throw std::runtime_error("Invalid proto encoder type to decode");
        }
    }
}

Encoder::position Encoder::from_proto(viam::component::encoder::v1::GetPositionResponse proto) {
    Encoder::position position;
    position.value = proto.value();

    position.type = from_proto(proto.position_type());
    return position;
}

Encoder::properties Encoder::from_proto(viam::component::encoder::v1::GetPropertiesResponse proto) {
    Encoder::properties properties;
    properties.ticks_count_supported = proto.ticks_count_supported();

    properties.angle_degrees_supported = proto.angle_degrees_supported();
    return properties;
}

viam::component::encoder::v1::PositionType Encoder::to_proto(position_type position_type) {
    switch (position_type) {
        case Encoder::position_type::unspecified: {
            return viam::component::encoder::v1::PositionType::POSITION_TYPE_UNSPECIFIED;
        }
        case Encoder::position_type::angle_degrees: {
            return viam::component::encoder::v1::POSITION_TYPE_ANGLE_DEGREES;
        }
        case Encoder::position_type::ticks_count: {
            return viam::component::encoder::v1::POSITION_TYPE_TICKS_COUNT;
        }
        default: {
            throw std::runtime_error("Invalid proto encoder type to encode");
        }
    }
}

viam::component::encoder::v1::GetPositionResponse Encoder::to_proto(position position) {
    viam::component::encoder::v1::GetPositionResponse proto;
    proto.set_value(position.value);

    proto.set_position_type(to_proto(position.type));
    return proto;
}

viam::component::encoder::v1::GetPropertiesResponse Encoder::to_proto(properties properties) {
    viam::component::encoder::v1::GetPropertiesResponse proto;
    proto.set_ticks_count_supported(properties.ticks_count_supported);

    proto.set_angle_degrees_supported(properties.angle_degrees_supported);
    return proto;
}

Encoder::Encoder(std::string name) : Component(std::move(name)){};

bool operator==(const Encoder::position& lhs, const Encoder::position& rhs) {
    return (lhs.value == rhs.value && lhs.type == rhs.type);
}

bool operator==(const Encoder::properties& lhs, const Encoder::properties& rhs) {
    return (lhs.ticks_count_supported == rhs.ticks_count_supported &&
            lhs.angle_degrees_supported == rhs.angle_degrees_supported);
}

namespace {
bool init() {
    Registry::register_resource(Encoder::static_api(), Encoder::resource_registration());
    return true;
};

// NOLINTNEXTLINE
const bool inited = init();
}  // namespace

}  // namespace sdk
}  // namespace viam
