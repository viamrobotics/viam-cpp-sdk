/// @file module/data_consumer.hpp
///
/// @brief Defines the `DataConsumer` helper class
#pragma once

#include <cstdint>

#include <chrono>
#include <string>
#include <vector>

#include <viam/sdk/app/data_client.hpp>

namespace viam {
namespace sdk {

/// @class DataConsumer data_consumer.hpp "module/data_consumer.hpp"
/// @brief A `DataConsumer` provides an interfaces for a connection to the Viam app which can be
/// used to query historical data for a module resource.
/// @remark This class deals exclusively with BSON as vectors of bytes; it is up to users to perform
/// BSON encoding/decoding with a library of their choice.
class DataConsumer {
   public:
    struct query_options {
        /// @brief How far back should the data be included.
        /// @remark This field is stored as milliseconds, but can be constructed with other
        /// durations. For example
        ///     query_options opts{std::chrono::hours{5}};
        std::chrono::milliseconds time_back;

        /// @brief Any additional query stages to be included after the default query.
        std::vector<DataClient::BSONBytes> additional_stages;
    };

    DataConsumer(DataClient& dc);

    /// @brief Query historical module data for @param resource.
    /** @remark The default query is constructed from environment variables and the arguments passed
     * as follows:
     * The default query is as follows:
     *     {
     *         "$match": {
     *             "part_id": {part_id},
     *             "component_name": {resource},
     *             "time_received": {
     *                 "$gte": {time_point}
     *             }
     *         }
     *     }
     *
     * where
     * - `part_id` is the value of the environment variable
     * - `resource` is @param resource
     * - `time_point` is the current time minus the `time_back` field from @param opts
     *
     * If any additional stages are specified in @param opts they will be added after the default
     * query above.
     */
    std::vector<DataClient::BSONBytes> query_tabular_data(const std::string& resource,
                                                          query_options opts);

   private:
    DataClient& client_;
};

}  // namespace sdk
}  // namespace viam
