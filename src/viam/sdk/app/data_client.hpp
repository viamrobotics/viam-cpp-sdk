#pragma once

#include <cstdint>

#include <memory>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <viam/sdk/app/viam_client.hpp>
#include <viam/sdk/app/data_client_proto_conversions.hpp>

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

    /// @brief Options for creating a sequence.
    struct CreateSequenceOptions {
        std::vector<SequenceResourceFilter> resources;
        std::vector<std::string> sequence_tags;
        boost::optional<time_pt> start_time;
        boost::optional<time_pt> end_time;
    };

    /// @brief Options for retrieving a sequence.
    struct GetSequenceOptions {
        std::string id;
    };

    /// @brief Options for updating a sequence.
    struct UpdateSequenceOptions {
        std::string id;
        boost::optional<std::vector<SequenceResourceFilter>> resources;
        boost::optional<std::vector<std::string>> sequence_tags;
        boost::optional<time_pt> start_time;
        boost::optional<time_pt> end_time;
        boost::optional<std::vector<std::string>> field_mask_paths;
    };

    /// @brief Options for deleting a sequence.
    struct DeleteSequenceOptions {
        std::string id;
    };

    /// @brief Options for listing sequences.
    struct ListSequencesOptions {
        boost::optional<std::string> page_token;
        boost::optional<uint32_t> page_size;
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

    /// @brief Create a new sequence.
    /// @param org_id The organization ID.
    /// @param opts Options for creating the sequence.
    /// @return The ID of the created sequence.
    std::string create_sequence(const std::string& org_id, const CreateSequenceOptions& opts);

    /// @brief Retrieve a sequence by ID.
    /// @param opts Options for retrieving the sequence.
    /// @return The retrieved sequence.
    Sequence get_sequence(const GetSequenceOptions& opts);

    /// @brief Update the mutable fields of a sequence.
    /// @param opts Options for updating the sequence.
    void update_sequence(const UpdateSequenceOptions& opts);

    /// @brief Delete a sequence by ID.
    /// @param opts Options for deleting the sequence.
    void delete_sequence(const DeleteSequenceOptions& opts);

    /// @brief List sequences for a given organization.
    /// @param org_id The organization ID.
    /// @param opts Options for listing sequences.
    /// @return A pair containing a vector of sequences and the next page token.
    std::pair<std::vector<Sequence>, std::string> list_sequences(
        const std::string& org_id, const ListSequencesOptions& opts);

    /// @brief Convenience overload for listing sequences with default options.
    std::pair<std::vector<Sequence>, std::string> list_sequences(const std::string& org_id);

   private:
    DataClient(const ViamChannel& channel);

    struct impl;

    std::unique_ptr<impl> pimpl_;
};

}  // namespace sdk
}  // namespace viam