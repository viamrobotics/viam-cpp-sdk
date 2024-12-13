#include <viam/sdk/common/utils.hpp>

#include <random>
#include <unordered_map>
#include <vector>

#include <google/protobuf/duration.pb.h>
#include <google/protobuf/timestamp.pb.h>
#include <grpcpp/client_context.h>

#include <boost/algorithm/string.hpp>
#include <boost/blank.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/optional/optional.hpp>

#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/common/private/utils.hpp>
#include <viam/sdk/common/private/version_metadata.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/registry/registry.hpp>

namespace viam {
namespace sdk {

bool operator==(const response_metadata& lhs, const response_metadata& rhs) {
    return lhs.captured_at == rhs.captured_at;
}

namespace proto_convert_details {

void to_proto<time_pt>::operator()(time_pt tp, google::protobuf::Timestamp* result) const {
    const std::chrono::nanoseconds since_epoch = tp.time_since_epoch();

    const auto sec_floor = std::chrono::duration_cast<std::chrono::seconds>(since_epoch);
    const std::chrono::nanoseconds nano_part = since_epoch - sec_floor;

    result->set_seconds(sec_floor.count());
    result->set_nanos(static_cast<int32_t>(nano_part.count()));
}

time_pt from_proto<google::protobuf::Timestamp>::operator()(
    const google::protobuf::Timestamp* timestamp) const {
    return time_pt{std::chrono::seconds{timestamp->seconds()} +
                   std::chrono::nanoseconds{timestamp->nanos()}};
}

void to_proto<std::chrono::microseconds>::operator()(std::chrono::microseconds duration,
                                                     google::protobuf::Duration* proto) const {
    namespace sc = std::chrono;

    const sc::seconds seconds = sc::duration_cast<sc::seconds>(duration);
    const sc::nanoseconds nanos = duration - seconds;

    proto->set_nanos(static_cast<int32_t>(nanos.count()));
    proto->set_seconds(seconds.count());
}

std::chrono::microseconds from_proto<google::protobuf::Duration>::operator()(
    const google::protobuf::Duration* proto) const {
    namespace sc = std::chrono;
    const sc::seconds seconds_part{proto->seconds()};
    const sc::nanoseconds nanos_part{proto->nanos()};

    const sc::microseconds from_seconds = sc::duration_cast<sc::microseconds>(seconds_part);
    sc::microseconds from_nanos = sc::duration_cast<sc::microseconds>(nanos_part);

    if ((nanos_part.count() < 0) && (from_nanos > nanos_part)) {
        from_nanos -= sc::microseconds(1);
    } else if ((nanos_part.count() > 0) && (from_nanos < nanos_part)) {
        from_nanos += sc::microseconds(1);
    }
    return from_seconds + from_nanos;
    return std::chrono::microseconds();
}

void to_proto<response_metadata>::operator()(const response_metadata& self,
                                             common::v1::ResponseMetadata* proto) const {
    *(proto->mutable_captured_at()) = v2::to_proto(self.captured_at);
}

response_metadata from_proto<common::v1::ResponseMetadata>::operator()(
    const common::v1::ResponseMetadata* proto) const {
    return {v2::from_proto(proto->captured_at())};
}

}  // namespace proto_convert_details

std::vector<unsigned char> string_to_bytes(const std::string& s) {
    std::vector<unsigned char> bytes(s.begin(), s.end());
    return bytes;
}

std::string bytes_to_string(const std::vector<unsigned char>& b) {
    std::string img_string(b.begin(), b.end());
    return img_string;
}

void set_logger_severity_from_args(int argc, char** argv) {
    if (argc >= 3 && strcmp(argv[2], "--log-level=debug") == 0) {
        boost::log::core::get()->set_filter(boost::log::trivial::severity >=
                                            boost::log::trivial::debug);
        return;
    }
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
}

std::string random_debug_key() {
    static const char alphanum[] = "abcdefghijklmnopqrstuvwxyz";
    static std::default_random_engine generator(
        std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(0, sizeof(alphanum) - 1);

    std::string key;
    key.reserve(6);

    for (int i = 0; i < 6; ++i) {
        key += alphanum[distribution(generator)];
    };

    return key;
}

ProtoStruct debug_map() {
    return debug_map(random_debug_key());
}

ProtoStruct debug_map(std::string debug_key) {
    ProtoStruct map;
    map.emplace(impl::debug_map_key, ProtoValue(std::move(debug_key)));

    return map;
}

void add_debug_entry(ProtoStruct& map, std::string debug_key) {
    map.emplace(impl::debug_map_key, ProtoValue(std::move(debug_key)));
}

void add_debug_entry(ProtoStruct& map) {
    add_debug_entry(map, random_debug_key());
}

ProtoStruct with_debug_entry(ProtoStruct&& map, std::string debug_key) {
    add_debug_entry(map, std::move(debug_key));
    return map;
}

ProtoStruct with_debug_entry(ProtoStruct&& map) {
    add_debug_entry(map);
    return map;
}

ClientContext::ClientContext() {
    set_client_ctx_authority_();
    add_viam_client_version_();
}

ClientContext::~ClientContext() = default;

ClientContext::operator const grpc::ClientContext*() const {
    return wrapped_context_.get();
}

ClientContext::operator grpc::ClientContext*() {
    return wrapped_context_.get();
}

void ClientContext::try_cancel() {
    wrapped_context_->TryCancel();
}

void ClientContext::set_debug_key(const std::string& debug_key) {
    wrapped_context_->AddMetadata("dtname", debug_key);
}

void ClientContext::set_client_ctx_authority_() {
    wrapped_context_->set_authority("viam-placeholder");
}

void ClientContext::add_viam_client_version_() {
    wrapped_context_->AddMetadata("viam_client", impl::k_version);
}

bool from_dm_from_extra(const ProtoStruct& extra) {
    auto pos = extra.find("fromDataManagement");
    if (pos != extra.end()) {
        const ProtoValue& value = pos->second;

        return (value.is_a<bool>() && value.get_unchecked<bool>());
    }
    return false;
}
std::pair<std::string, std::string> long_name_to_remote_and_short(const std::string& long_name) {
    std::vector<std::string> name_parts;
    // boost::split causes a clang-tidy false positive, see
    // https://bugs.llvm.org/show_bug.cgi?id=41141
    //
    // NOLINTNEXTLINE
    boost::split(name_parts, long_name, boost::is_any_of(":"));
    auto name = name_parts.back();
    name_parts.pop_back();
    auto remote_name = name_parts.empty()
                           ? ""
                           : std::accumulate(std::next(name_parts.begin()),
                                             name_parts.end(),
                                             *name_parts.begin(),
                                             [](const std::string& a, const std::string& b) {
                                                 return a + ":" + b;
                                             });

    return {std::move(remote_name), std::move(name)};
}

}  // namespace sdk
}  // namespace viam
