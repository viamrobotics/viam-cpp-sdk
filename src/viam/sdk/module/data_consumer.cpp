#include <viam/sdk/module/data_consumer.hpp>

#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

DataConsumer::DataConsumer(DataClient& dc) : client_(dc) {}

std::vector<DataClient::MQLBinary> DataConsumer::query_tabular_data(
    const std::string& resource, DataConsumer::QueryOptions opts) {
    const std::string org_id = get_env("VIAM_PRIMARY_ORG_ID");
    const std::string part_id = get_env("VIAM_MACHINE_PART_ID");

    DataClient::MQLBinary base_query;

    opts.additional_stages.insert(opts.additional_stages.begin(), std::move(base_query));

    return client_.tabular_data_by_mql(org_id, opts.additional_stages, {});
}

}  // namespace sdk
}  // namespace viam
