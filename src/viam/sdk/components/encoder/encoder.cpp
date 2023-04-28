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

std::shared_ptr<ResourceServerBase> EncoderSubtype::create_resource_server(
    std::shared_ptr<SubtypeService> svc) {
    return std::make_shared<EncoderServer>(svc);
};

std::shared_ptr<ResourceBase> EncoderSubtype::create_rpc_client(
    std::string name, std::shared_ptr<grpc::Channel> chan) {
    return std::make_shared<EncoderClient>(std::move(name), std::move(chan));
};

std::shared_ptr<ResourceSubtype> Encoder::resource_subtype() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::encoder::v1::EncoderService::service_full_name());
    if (sd == nullptr) {
        throw std::runtime_error("Unable to get service descriptor for the encoder service");
    }
    return std::make_shared<EncoderSubtype>(sd);
}

Subtype Encoder::subtype() {
    return Subtype(RDK, COMPONENT, "encoder");
}

Encoder::position_type Encoder::from_proto(viam::component::encoder::v1::PositionType proto) {
    switch (proto) {
        case viam::component::encoder::v1::POSITION_TYPE_UNSPECIFIED: {
            return Encoder::position_type::UNSPECIFIED;
        }
        case viam::component::encoder::v1::POSITION_TYPE_ANGLE_DEGREES: {
            return Encoder::position_type::ANGLE_DEGREES;
        }
        case viam::component::encoder::v1::POSITION_TYPE_TICKS_COUNT: {
            return Encoder::position_type::TICKS_COUNT;
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
        case Encoder::position_type::UNSPECIFIED: {
        }
        case Encoder::position_type::ANGLE_DEGREES: {
            return viam::component::encoder::v1::POSITION_TYPE_ANGLE_DEGREES;
        }
        case Encoder::position_type::TICKS_COUNT: {
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

bool operator==(const Encoder::position& lhs, const Encoder::position& rhs) {
    return (lhs.value == rhs.value && lhs.type == rhs.type);
}

bool operator==(const Encoder::properties& lhs, const Encoder::properties& rhs) {
    return (lhs.ticks_count_supported == rhs.ticks_count_supported &&
            lhs.angle_degrees_supported == rhs.angle_degrees_supported);
}

namespace {
bool init() {
    Registry::register_subtype(Encoder::subtype(), Encoder::resource_subtype());
    return true;
};

bool inited = init();
}  // namespace

}  // namespace sdk
}  // namespace viam
