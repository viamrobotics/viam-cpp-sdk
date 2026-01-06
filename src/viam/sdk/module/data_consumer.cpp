#include <viam/sdk/module/data_consumer.hpp>

#include <cstdint>

#include <algorithm>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/module/private/data_consumer_query.hpp>

namespace viam {
namespace sdk {

DataConsumer::DataConsumer(DataClient& dc) : client_(dc) {}

std::vector<DataClient::BSONBytes> DataConsumer::query_tabular_data(
    const std::string& resource, DataConsumer::query_options opts) {
    const std::string org_id = get_env("VIAM_PRIMARY_ORG_ID").value_or("");
    const std::string part_id = get_env("VIAM_MACHINE_PART_ID").value_or("");

    auto time_point = std::chrono::time_point_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - opts.time_back);

    DataClient::BSONBytes base_query =
        sdk::impl::default_historical_data_query(part_id, resource, time_point);

    opts.additional_stages.insert(opts.additional_stages.begin(), std::move(base_query));

    return client_.tabular_data_by_mql(org_id, opts.additional_stages);
}

}  // namespace sdk
}  // namespace viam
