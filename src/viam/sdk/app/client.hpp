#include <viam/sdk/rpc/dial.hpp>

#include <vector>

#include <cstdint>

namespace viam {
namespace sdk {

class AppClient {
   public:
    enum class TabularDataSourceType { standard, hot_storage, pipeline_sink };

    static AppClient connect(const char* uri,
                             const Credentials& credentials,
                             const std::string& auth_entity);

    const ViamChannel& channel() const;

    std::vector<std::vector<std::uint8_t>> tabular_data_by_mql(
        const std::string& org_id,
        const std::vector<std::vector<std::uint8_t>>& mql_binary,
        TabularDataSourceType src_type,
        const std::string& pipeline_id,
        const std::string& query_prefix);

   private:
    AppClient(ViamChannel channel);

    ViamChannel channel_;
};

}  // namespace sdk
}  // namespace viam
