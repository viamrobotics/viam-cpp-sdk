#pragma once

#include <cstdint>

#include <chrono>
#include <vector>

#include <viam/sdk/app/data_client.hpp>

namespace viam {
namespace sdk {

class DataConsumer {
   public:
    struct query_options {
        /// @brief How far back should the data be included.
        std::chrono::milliseconds time_back;

        /// @brief Any additional query stages to be included after the default query.
        std::vector<DataClient::BSONBytes> additional_stages;
    };

    static DataClient::BSONBytes default_query(
        const std::string& part_id,
        std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> time_point);

    DataConsumer(DataClient& dc);

    std::vector<DataClient::BSONBytes> query_tabular_data(const std::string& resource,
                                                          query_options opts);

   private:
    DataClient& client_;
};

}  // namespace sdk
}  // namespace viam
