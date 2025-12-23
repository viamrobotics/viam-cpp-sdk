#include <viam/sdk/module/data_consumer.hpp>

#include <cstdint>

#include <algorithm>

#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/common/proto_value_visit.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

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

        *reinterpret_cast<int32_t*>(&buf.front()) = buf.size();
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

DataClient::BSONBytes DataConsumer::default_query(
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

DataConsumer::DataConsumer(DataClient& dc) : client_(dc) {}

std::vector<DataClient::BSONBytes> DataConsumer::query_tabular_data(
    const std::string& resource, DataConsumer::query_options opts) {
    const std::string org_id = get_env("VIAM_PRIMARY_ORG_ID").value_or("");
    const std::string part_id = get_env("VIAM_MACHINE_PART_ID").value_or("");

    auto time_point = std::chrono::time_point_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - opts.time_back);

    DataClient::BSONBytes base_query = default_query(part_id, resource, time_point);

    opts.additional_stages.insert(opts.additional_stages.begin(), std::move(base_query));

    return client_.tabular_data_by_mql(org_id, opts.additional_stages);
}

}  // namespace sdk
}  // namespace viam
