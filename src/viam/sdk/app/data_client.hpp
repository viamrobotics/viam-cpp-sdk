#pragma once

#include <cstdint>

#include <memory>
#include <string>
#include <vector>

#include <viam/sdk/app/viam_client.hpp>

namespace viam {
namespace sdk {

class DataClient {
   public:
    enum class TabularDataSourceType { standard, hot_storage, pipeline_sink };

    DataClient(const ViamChannel& channel);

    ~DataClient();

    const ViamChannel& channel() const;

    std::vector<std::vector<std::uint8_t>> tabular_data_by_mql(
        const std::string& org_id,
        const std::vector<std::vector<std::uint8_t>>& mql_binary,
        TabularDataSourceType src_type,
        const std::string& pipeline_id,
        const std::string& query_prefix);

   private:
    struct impl;

    std::unique_ptr<impl> pimpl_;
};

}  // namespace sdk
}  // namespace viam
