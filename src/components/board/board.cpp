#include <components/board/board.hpp>

#include <components/board/board.hpp>

#include <google/protobuf/descriptor.h>

#include <common/v1/common.pb.h>
#include <component/board/v1/board.grpc.pb.h>
#include <component/board/v1/board.pb.h>

#include <common/utils.hpp>
#include <components/board/client.hpp>
#include <components/board/server.hpp>
#include <registry/registry.hpp>
#include <resource/resource.hpp>
std::shared_ptr<ResourceServerBase> BoardSubtype::create_resource_server(
    std::shared_ptr<SubtypeService> service) {
    return std::make_shared<BoardServer>(service);
}

std::shared_ptr<ResourceBase> BoardSubtype::create_rpc_client(
    std::string name, std::shared_ptr<grpc::Channel> channel) {
    return std::make_shared<BoardClient>(std::move(name), std::move(channel));
}

std::shared_ptr<ResourceSubtype> Board::resource_subtype() {
    const auto descriptor_pool = google::protobuf::DescriptorPool::generated_pool();
    const auto service_descriptor = descriptor_pool->FindServiceByName(
        viam::component::board::v1::BoardService::service_full_name());
    if (!service_descriptor) {
        throw std::runtime_error("Unable to get service descriptor for the board service");
    }
    return std::make_shared<BoardSubtype>(service_descriptor);
}

Subtype Board::subtype() {
    return Subtype(RDK, COMPONENT, "board");
}

Board::analog_status Board::from_proto(viam::common::v1::AnalogStatus proto) {
    return {proto.value()};
}

Board::digital_interrupt_status Board::from_proto(viam::common::v1::DigitalInterruptStatus proto) {
    return {proto.value()};
}

Board::board_status Board::from_proto(viam::common::v1::BoardStatus proto) {
    board_status result;
    for (const auto& analog : proto.analogs()) {
        result.analogs.emplace(analog.first, from_proto(analog.second));
    }
    for (const auto& digital_interrupt : proto.digital_interrupts()) {
        result.digital_interrupts.emplace(digital_interrupt.first,
                                          from_proto(digital_interrupt.second));
    }
    return result;
}

Board::power_mode Board::from_proto(viam::component::board::v1::PowerMode proto) {
    return static_cast<power_mode>(proto);
}

viam::common::v1::AnalogStatus Board::to_proto(Board::analog_status analog) {
    viam::common::v1::AnalogStatus proto;
    proto.set_value(analog.value);
    return proto;
}

viam::common::v1::DigitalInterruptStatus Board::to_proto(
    Board::digital_interrupt_status digital_interrupt) {
    viam::common::v1::DigitalInterruptStatus proto;
    proto.set_value(digital_interrupt.value);
    return proto;
}

viam::common::v1::BoardStatus Board::to_proto(Board::board_status board) {
    viam::common::v1::BoardStatus proto;
    for (const auto& analog : board.analogs) {
        proto.mutable_analogs()->insert({analog.first, to_proto(analog.second)});
    }
    for (const auto& digital_interrupt : board.digital_interrupts) {
        proto.mutable_digital_interrupts()->insert(
            {digital_interrupt.first, to_proto(digital_interrupt.second)});
    }
    return proto;
}

viam::component::board::v1::PowerMode Board::to_proto(Board::power_mode power) {
    return static_cast<viam::component::board::v1::PowerMode>(power);
}

bool operator==(const Board::analog_status& left, const Board::analog_status& right) {
    return left.value == right.value;
}

bool operator==(const Board::digital_interrupt_status& left,
                const Board::digital_interrupt_status& right) {
    return left.value == right.value;
}

bool operator==(const Board::board_status& left, const Board::board_status& right) {
    return left.analogs == right.analogs && left.digital_interrupts == right.digital_interrupts;
}

namespace {
bool init() {
    Registry::register_subtype(Board::subtype(), Board::resource_subtype());
    return true;
};

bool inited = init();
}  // namespace
