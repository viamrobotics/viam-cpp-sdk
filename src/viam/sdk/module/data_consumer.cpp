#include <viam/sdk/module/data_consumer.hpp>

#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

struct writer {
    void write_string(const std::string& value) {}
    DataClient::BSONBytes bytes;
};

DataClient::BSONBytes DataConsumer::default_query(
    const std::string& part_id,
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
}

DataConsumer::DataConsumer(DataClient& dc) : client_(dc) {}

std::vector<DataClient::BSONBytes> DataConsumer::query_tabular_data(
    const std::string& resource, DataConsumer::query_options opts) {
    const std::string org_id = get_env("VIAM_PRIMARY_ORG_ID").value_or("");
    const std::string part_id = get_env("VIAM_MACHINE_PART_ID").value_or("");

    DataClient::BSONBytes base_query;

    opts.additional_stages.insert(opts.additional_stages.begin(), std::move(base_query));

    return client_.tabular_data_by_mql(org_id, opts.additional_stages, {});
}

}  // namespace sdk
}  // namespace viam
