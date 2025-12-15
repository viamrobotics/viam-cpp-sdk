#pragma once

#include <cstdint>

#include <memory>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <viam/sdk/app/viam_client.hpp>

namespace viam {
namespace sdk {

class DataClient {
   public:
    enum class TabularDataSourceType { k_standard, k_hot_storage, k_pipeline_sink };

    /// @brief Options which are passed to a tabular_data_by_mql request.
    struct TabularDataByMQLOpts {
        /// @brief Source type for the query.
        TabularDataSourceType src_type = TabularDataSourceType::k_hot_storage;

        /// @brief Pipeline ID for requests with pipeline sink source type.
        boost::optional<std::string> pipeline_id;

        /// @brief Used to specify a saved query to run.
        boost::optional<std::string> query_prefix;
    };

    using BSONBytes = std::vector<uint8_t>;

    static DataClient from_viam_client(const ViamClient&);

    DataClient(DataClient&&) = default;

    DataClient& operator=(DataClient&&) = default;

    ~DataClient();

    const ViamChannel& channel() const;

    /// @brief Request tabular data using an MQL query.
    /// @param mql_binary A MongoDB aggregation pipeline as a list of BSON documents, each
    /// representing one stage in the pipeline.
    /// @return BSON documents of unified tabular data and metadata.
    /// @remark This interface deals exclusively with BSON as vectors of bytes; it is up to users
    /// to perform BSON encoding/decoding with their library of choice.
    std::vector<BSONBytes> tabular_data_by_mql(const std::string& org_id,
                                               const std::vector<BSONBytes>& mql_binary,
                                               const TabularDataByMQLOpts& opts);

    /// @brief Convenience overload with default options.
    std::vector<BSONBytes> tabular_data_by_mql(const std::string& org_id,
                                               const std::vector<BSONBytes>& mql_binary);

   private:
    DataClient(const ViamChannel& channel);

    struct impl;

    std::unique_ptr<impl> pimpl_;
};

}  // namespace sdk
}  // namespace viam
