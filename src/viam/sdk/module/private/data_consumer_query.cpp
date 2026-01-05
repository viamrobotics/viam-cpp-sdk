#include <viam/sdk/module/private/data_consumer_query.hpp>

#include <cstdint>

#include <algorithm>
#include <iterator>
#include <vector>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/proto_value_visit.hpp>

namespace viam {
namespace sdk {
namespace impl {

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

    void write_header(int8_t object_id, const std::string& key) {
        write_bytes(object_id);
        write_cstring(key);
    }

    void write_bson_doc(const ProtoStruct& top_level) {
        // First four bytes are total object size; push it here then calculate it at the end
        write_bytes(int32_t{});

        for (const auto& item : top_level) {
            const std::string& key = item.first;

            auto visitor = [&key, this](const auto& val) { write_entry(key, val); };

            visit(visitor, item.second);
        }

        write_bytes(uint8_t{0});  // end of object

        *reinterpret_cast<int32_t*>(&buf.front()) = static_cast<int32_t>(buf.size());
    }

    void write_entry(const std::string& key, const std::string& val) {
        write_header(int8_t{2}, key);

        write_bytes(static_cast<int32_t>(val.size() + 1U));  // string size + null byte

        write_cstring(val);
    }

    void write_entry(const std::string& key, const ProtoStruct& doc) {
        write_header(int8_t{3}, key);

        writer inner;
        inner.write_bson_doc(doc);

        buf.insert(buf.end(), inner.buf.begin(), inner.buf.end());
    }

    void write_entry(const std::string& key, double timestamp) {
        write_header(int8_t{9}, key);

        write_bytes(static_cast<int64_t>(timestamp));
    }

    template <typename T>
    void write_entry(const std::string&, const T&) {
        throw std::logic_error("unexpected visitor case");
    }
};

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
