#pragma once

#include <cstdint>

#include <chrono>
#include <vector>

#include <viam/sdk/app/data_client.hpp>

namespace viam {
namespace sdk {

class DataConsumer {
   public:
    struct QueryOptions {
        /// @brief How far back should the data be included.
        std::chrono::milliseconds time_back;

        /// @brief Any additional query stages to be included after the default query.
        std::vector<DataClient::MQLBinary> additional_stages;
    };

    DataConsumer(DataClient& dc);

    std::vector<DataClient::MQLBinary> query_tabular_data(const std::string& resource,
                                                          QueryOptions opts);

   private:
    DataClient& client_;
};

}  // namespace sdk
}  // namespace viam
