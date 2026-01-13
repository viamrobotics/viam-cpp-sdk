#pragma once

#include <chrono>
#include <string>

#include <viam/sdk/app/data_client.hpp>

namespace viam {
namespace sdk {
namespace impl {

DataClient::BSONBytes default_historical_data_query(
    const std::string& part_id,
    const std::string& resource,
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> time_point);

}  // namespace impl
}  // namespace sdk
}  // namespace viam
