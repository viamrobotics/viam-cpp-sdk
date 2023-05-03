#include <viam/sdk/components/board/board.hpp>

#include <google/protobuf/descriptor.h>

#include <viam/api/component/board/v1/board.grpc.pb.h>
#include <viam/api/component/board/v1/board.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/board/client.hpp>
#include <viam/sdk/components/board/server.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>

namespace viam {
namespace sdk {

BoardSubtype::BoardSubtype(const google::protobuf::ServiceDescriptor* service_descriptor)
    : ResourceSubtype(service_descriptor) {}

std::shared_ptr<ResourceServer> BoardSubtype::create_resource_server(
    std::shared_ptr<ResourceManager> manager) {
    return std::make_shared<BoardServer>(manager);
}

std::shared_ptr<Resource> BoardSubtype::create_rpc_client(std::string name,
                                                          std::shared_ptr<grpc::Channel> chan) {
    return std::make_shared<BoardClient>(std::move(name), std::move(chan));
}

std::shared_ptr<ResourceSubtype> Board::resource_subtype() {
    const google::protobuf::DescriptorPool* p = google::protobuf::DescriptorPool::generated_pool();
    const google::protobuf::ServiceDescriptor* sd =
        p->FindServiceByName(viam::component::board::v1::BoardService::service_full_name());
    if (!sd) {
        throw std::runtime_error("Unable to get service descriptor for the board service");
    }
    return std::make_shared<BoardSubtype>(sd);
}

Subtype Board::subtype() {
    return Subtype(RDK, COMPONENT, "board");
}

Board::status Board::from_proto(viam::common::v1::BoardStatus proto) {
    Board::status status;
    for (const auto& analog : proto.analogs()) {
        status.analog_reader_values.emplace(analog.first, analog.second.value());
    }
    for (const auto& digital : proto.digital_interrupts()) {
        status.digital_interrupt_values.emplace(digital.first, digital.second.value());
    }
    return status;
}

Board::analog_value Board::from_proto(viam::common::v1::AnalogStatus proto) {
    return proto.value();
}

Board::digital_value Board::from_proto(viam::common::v1::DigitalInterruptStatus proto) {
    return proto.value();
}

Board::power_mode Board::from_proto(viam::component::board::v1::PowerMode proto) {
    switch (proto) {
        case viam::component::board::v1::POWER_MODE_NORMAL: {
            return Board::power_mode::normal;
        }
        case viam::component::board::v1::POWER_MODE_OFFLINE_DEEP: {
            return Board::power_mode::offline_deep;
        }
        case viam::component::board::v1::POWER_MODE_UNSPECIFIED:
        default: {
            throw std::runtime_error("Invalid proto board power_mode to decode");
        }
    }
}

viam::common::v1::BoardStatus Board::to_proto(status status) {
    viam::common::v1::BoardStatus proto;
    for (const auto& analog : status.analog_reader_values) {
        proto.mutable_analogs()->insert({analog.first, to_proto(analog.second)});
    }

    for (const auto& digital : status.digital_interrupt_values) {
        proto.mutable_digital_interrupts()->insert({digital.first, to_proto(digital.second)});
    }
    return proto;
}

viam::common::v1::AnalogStatus Board::to_proto(analog_value analog_value) {
    viam::common::v1::AnalogStatus proto;
    proto.set_value(analog_value);
    return proto;
}

viam::common::v1::DigitalInterruptStatus Board::to_proto(digital_value digital_value) {
    viam::common::v1::DigitalInterruptStatus proto;
    proto.set_value(digital_value);
    return proto;
}

viam::component::board::v1::PowerMode Board::to_proto(Board::power_mode proto) {
    switch (proto) {
        case Board::power_mode::normal: {
            return viam::component::board::v1::POWER_MODE_NORMAL;
        }
        case Board::power_mode::offline_deep: {
            return viam::component::board::v1::POWER_MODE_OFFLINE_DEEP;
        }
        default: {
            throw std::runtime_error("Invalid board power_mode to encode");
        }
    }
}

Board::Board(std::string name) : Component(std::move(name)){};

bool operator==(const Board::status& lhs, const Board::status& rhs) {
    return (lhs.analog_reader_values == rhs.analog_reader_values &&
            lhs.digital_interrupt_values == rhs.digital_interrupt_values);
}

namespace {
bool init() {
    Registry::register_subtype(Board::subtype(), Board::resource_subtype());
    return true;
};

const bool inited = init();
}  // namespace

}  // namespace sdk
}  // namespace viam
