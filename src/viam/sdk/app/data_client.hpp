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

    struct TabularDataByMQLOpts {
        TabularDataSourceType src_type = TabularDataSourceType::hot_storage;
        std::string pipeline_id;
        std::string query_prefix;
    };

    static DataClient from_viam_client(const ViamClient&);

    DataClient(DataClient&&) = default;

    DataClient& operator=(DataClient&&) = default;

    ~DataClient();

    const ViamChannel& channel() const;

    std::vector<std::vector<std::uint8_t>> tabular_data_by_mql(
        const std::string& org_id,
        const std::vector<std::vector<std::uint8_t>>& mql_binary,
        const TabularDataByMQLOpts& opts);

   private:
    DataClient(const ViamChannel& channel);

    struct impl;

    std::unique_ptr<impl> pimpl_;
};

}  // namespace sdk
}  // namespace viam
