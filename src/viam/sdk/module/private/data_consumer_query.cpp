#include <viam/sdk/module/private/data_consumer_query.hpp>

#include <cstdint>

#include <algorithm>
#include <iterator>
#include <vector>

#include <boost/endian/conversion.hpp>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/proto_value_visit.hpp>

namespace viam {
namespace sdk {
namespace impl {

namespace {

namespace endian = boost::endian;

enum class object_type : int8_t { k_string = 2, k_document = 3, k_datetime = 9 };

struct writer {
    std::vector<uint8_t> buf;

    template <typename T>
    void write_bytes(T value) {
        std::copy_n(reinterpret_cast<uint8_t*>(&value), sizeof(T), std::back_inserter(buf));
    }

    void write_cstring(const std::string& val) {
        std::copy_n(
            reinterpret_cast<uint8_t const*>(val.data()), val.size() + 1U, std::back_inserter(buf));
    }

    void write_header(object_type object_id, const std::string& key) {
        write_bytes(static_cast<int8_t>(object_id));
        write_cstring(key);
    }

    void write_doc_size() {
        const auto doc_size = endian::native_to_little(static_cast<int32_t>(buf.size()));

        std::copy_n(reinterpret_cast<uint8_t const*>(&doc_size), sizeof(doc_size), buf.begin());
    }

    void write_bson_doc(const ProtoStruct& top_level) {  // NOLINT(misc-no-recursion)
        // First four bytes are total object size; push it here then calculate it at the end
        write_bytes(int32_t{});

        for (const auto& item : top_level) {
            const std::string& key = item.first;

            auto visitor = [&key, this](const auto& val) {  // NOLINT(misc-no-recursion)
                write_entry(key, val);
            };

            visit(visitor, item.second);
        }

        write_bytes(uint8_t{0});  // end of object
        write_doc_size();
    }

    void write_entry(const std::string& key, const std::string& val) {
        write_header(object_type::k_string, key);

        write_bytes(endian::native_to_little(
            static_cast<int32_t>(val.size() + 1U)));  // string size + null byte

        write_cstring(val);
    }

    void write_entry(const std::string& key, const ProtoStruct& doc) {  // NOLINT(misc-no-recursion)
        write_header(object_type::k_document, key);

        writer inner;
        inner.write_bson_doc(doc);

        buf.insert(buf.end(), inner.buf.begin(), inner.buf.end());
    }

    void write_entry(const std::string& key, double timestamp) {
        write_header(object_type::k_datetime, key);

        write_bytes(endian::native_to_little(static_cast<int64_t>(timestamp)));
    }

    template <typename T>
    void write_entry(const std::string&, const T&) {
        throw std::logic_error("unexpected visitor case");
    }
};

}  // namespace

DataClient::BSONBytes default_historical_data_query(
    const std::string& part_id,
    const std::string& resource,
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> time_point) {
    /*
        The default query is as follows:
        {
            "$match": {
                "part_id": {part_id},
                "component_name": {resource_name},
                "time_received": {
                    "$gte": {time_point}
                }
            }
        }
    */

    auto query = ProtoStruct{
        {{"$match",
          ProtoStruct{
              {{"part_id", part_id},          //
               {"component_name", resource},  //
               {"time_received",
                ProtoStruct{
                    {{"$gte", static_cast<double>(time_point.time_since_epoch().count())}}  //
                }}}}}}};

    writer w;

    w.write_bson_doc(query);

    return w.buf;
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
