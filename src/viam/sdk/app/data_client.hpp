#pragma once

#include <cstdint>

#include <memory>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <viam/sdk/app/viam_client.hpp>
#include <viam/sdk/app/data_client_types.hpp>

namespace viam {
namespace sdk {

class DataClient {
   public:
    enum class TabularDataSourceType { k_standard, k_hot_storage, k_pipeline_sink };

    /// @brief Options which are passed to a tabular_data_by_mql request.
    struct tabular_data_by_mql_opts {
        /// @brief Source type for the query.
        TabularDataSourceType src_type = TabularDataSourceType::k_hot_storage;

        /// @brief Pipeline ID for requests with pipeline sink source type.
        boost::optional<std::string> pipeline_id;

        /// @brief Used to specify a saved query to run.
        boost::optional<std::string> query_prefix;
    };

    // NEW: Options for SequencesByDatasetID
    /// @brief Options which are passed to a sequences_by_dataset_id request.
    struct sequences_by_dataset_id_options {
        /// @brief The number of sequences to return per page.
        uint32_t page_size = 0;
        /// @brief The token for the next page of results.
        boost::optional<std::string> page_token;
    };

    using BSONBytes = std::vector<uint8_t>;

    static DataClient from_viam_client(const ViamClient&);

    DataClient(DataClient&&) noexcept;

    DataClient& operator=(DataClient&&) noexcept;

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
                                               const tabular_data_by_mql_opts& opts);

    /// @brief Convenience overload with default options.
    std::vector<BSONBytes> tabular_data_by_mql(const std::string& org_id,
                                               const std::vector<BSONBytes>& mql_binary);

    // NEW: AddSequencesToDataset
    /// @brief Adds the sequences with the given IDs to the dataset.
    /// @param dataset_id The ID of the dataset to add sequences to.
    /// @param sequence_ids The IDs of the sequences to add.
    void add_sequences_to_dataset(const std::string& dataset_id,
                                  const std::vector<std::string>& sequence_ids);

    // NEW: RemoveSequencesFromDataset
    /// @brief Removes the sequences with the given IDs from the dataset.
    /// @param dataset_id The ID of the dataset to remove sequences from.
    /// @param sequence_ids The IDs of the sequences to remove.
    void remove_sequences_from_dataset(const std::string& dataset_id,
                                       const std::vector<std::string>& sequence_ids);

    // NEW: SequencesByDatasetID
    /// @brief Lists sequences that belong to the given dataset.
    /// @param dataset_id The ID of the dataset to list sequences for.
    /// @param opts Options for pagination.
    /// @return A pair containing a vector of sequences and the next page token.
    std::pair<std::vector<Sequence>, boost::optional<std::string>> sequences_by_dataset_id(
        const std::string& dataset_id, const sequences_by_dataset_id_options& opts);

    // NEW: Convenience overload for SequencesByDatasetID with default options.
    /// @brief Convenience overload for SequencesByDatasetID with default options.
    std::pair<std::vector<Sequence>, boost::optional<std::string>> sequences_by_dataset_id(
        const std::string& dataset_id);

   private:
    DataClient(const ViamChannel& channel);

    struct impl;

    std::unique_ptr<impl> pimpl_;
};

}  // namespace sdk
}  // namespace viam