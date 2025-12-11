#include <viam/sdk/app/data_client.hpp>

#include <boost/config.hpp>

#include <grpcpp/channel.h>

#include <viam/api/app/data/v1/data.grpc.pb.h>
#include <viam/api/app/data/v1/data.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

using viam::app::data::v1::DataService;

struct DataClient::impl {
    impl(const ViamChannel& c) : chan(&c), stub(DataService::NewStub(c.channel())) {}

    const ViamChannel& channel() const {
        return *chan;
    }

    template <typename Method>
    auto client_helper(Method m) {
        return make_client_helper(this, *stub, m);
    }

    const ViamChannel* chan;
    std::unique_ptr<DataService::Stub> stub;
};

DataClient DataClient::from_viam_client(const ViamClient& client) {
    return DataClient(client.channel());
}

DataClient::DataClient(const ViamChannel& channel) : pimpl_(std::make_unique<impl>(channel)) {}

DataClient::~DataClient() = default;

const ViamChannel& DataClient::channel() const {
    return pimpl_->channel();
}

std::vector<std::vector<std::uint8_t>> DataClient::tabular_data_by_mql(
    const std::string& org_id,
    const std::vector<std::vector<std::uint8_t>>& mql_binary,
    const DataClient::TabularDataByMQLOpts& opts) {
    return pimpl_->client_helper(&DataService::Stub::TabularDataByMQL)
        .with([&](app::data::v1::TabularDataByMQLRequest& req) {
            req.set_organization_id(org_id);

            for (const auto& query : mql_binary) {
                *(req.mutable_mql_binary()->Add()) = bytes_to_string(query);
            }

            switch (opts.src_type) {
                case TabularDataSourceType::standard:
                    req.mutable_data_source()->set_type(
                        app::data::v1::TABULAR_DATA_SOURCE_TYPE_STANDARD);
                    break;
                case TabularDataSourceType::hot_storage:
                    BOOST_FALLTHROUGH;
                default:
                    req.mutable_data_source()->set_type(
                        app::data::v1::TABULAR_DATA_SOURCE_TYPE_HOT_STORAGE);
                    break;
                case TabularDataSourceType::pipeline_sink:
                    req.mutable_data_source()->set_type(
                        app::data::v1::TABULAR_DATA_SOURCE_TYPE_PIPELINE_SINK);
            }

            if (!opts.pipeline_id.empty()) {
                req.mutable_data_source()->set_pipeline_id(opts.pipeline_id);
            }

            if (!opts.query_prefix.empty()) {
                req.set_query_prefix_name(opts.query_prefix);
            }
        })
        .invoke([](const app::data::v1::TabularDataByMQLResponse& resp) {
            std::vector<std::vector<uint8_t>> result;

            for (const auto& str : resp.raw_data()) {
                result.push_back(string_to_bytes(str));
            }

            return result;
        });
}

}  // namespace sdk
}  // namespace viam
