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
    struct tabular_data_by_mql_opts {
        /// @brief Source type for the query.
        TabularDataSourceType src_type = TabularDataSourceType::k_hot_storage;

        /// @brief Pipeline ID for requests with pipeline sink source type.
        boost::optional<std::string> pipeline_id;

        /// @brief Used to specify a saved query to run.
        boost::optional<std::string> query_prefix;
    };

    struct SequenceResourceFilter {
        std::string resource_name;
        std::string method_name;

        bool operator==(const SequenceResourceFilter& other) const = default;
    };

    struct Sequence {
        std::string id;
        std::string part_id;
        std::vector<std::string> sequence_tags;
        std::chrono::system_clock::time_point created_at;
        std::chrono::system_clock::time_point updated_at;
        std::chrono::system_clock::time_point start_time;
        std::chrono::system_clock::time_point end_time;
        std::vector<SequenceResourceFilter> resources;

        bool operator==(const Sequence& other) const = default;
    };

    struct update_sequence_options {
        boost::optional<std::vector<SequenceResourceFilter>> resources;
        boost::optional<std::vector<std::string>> sequence_tags;
        boost::optional<std::chrono::system_clock::time_point> start_time;
        boost::optional<std::chrono::system_clock::time_point> end_time;
        std::vector<std::string> field_mask;
    };

    struct list_sequences_options {
        boost::optional<std::string> page_token;
        boost::optional<uint32_t> page_size;
    };

    // CreateSequence creates a new sequence.
    std::string CreateSequence(const std::string& part_id,
                               const std::vector<SequenceResourceFilter>& resources,
                               const std::vector<std::string>& sequence_tags,
                               const boost::optional<std::chrono::system_clock::time_point>& start_time,
                               const boost::optional<std::chrono::system_clock::time_point>& end_time);

    // Convenience overload with default options.
    std::string CreateSequence(const std::string& part_id,
                               const std::vector<SequenceResourceFilter>& resources,
                               const std::vector<std::string>& sequence_tags);

    // GetSequence retrieves a sequence by ID.
    Sequence GetSequence(const std::string& id);

    // UpdateSequence updates the mutable fields of a sequence.
    void UpdateSequence(const std::string& id, const update_sequence_options& opts);

    // DeleteSequence deletes a sequence by ID.
    void DeleteSequence(const std::string& id);

    // ListSequences lists sequences for a given organization.
    std::pair<std::vector<Sequence>, std::string> ListSequences(const std::string& organization_id,
                                                                const list_sequences_options& opts);

    // Convenience overload with default options.
    std::pair<std::vector<Sequence>, std::string> ListSequences(const std::string& organization_id);


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

   private:
    DataClient(const ViamChannel& channel);

    struct impl;

    std::unique_ptr<impl> pimpl_;
};

}  // namespace sdk
}  // namespace viam