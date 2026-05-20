#include <viam/sdk/app/data_client.hpp>

#include <boost/config.hpp>

#include <grpcpp/channel.h>

#include <viam/api/app/data/v1/data.grpc.pb.h>
#include <viam/api/app/data/v1/data.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/common/private/repeated_ptr_convert.hpp>
#include <viam/sdk/app/data_client_types.hpp>

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

DataClient::DataClient(DataClient&&) noexcept = default;
DataClient& DataClient::operator=(DataClient&&) noexcept = default;

DataClient::~DataClient() = default;

const ViamChannel& DataClient::channel() const {
    return pimpl_->channel();
}

std::vector<DataClient::BSONBytes> DataClient::tabular_data_by_mql(
    const std::string& org_id,
    const std::vector<DataClient::BSONBytes>& mql_binary,
    const DataClient::tabular_data_by_mql_opts& opts) {
    return pimpl_->client_helper(&DataService::Stub::TabularDataByMQL)
        .with([&](app::data::v1::TabularDataByMQLRequest& req) {
            req.set_organization_id(org_id);

            for (const auto& query : mql_binary) {
                *(req.mutable_mql_binary()->Add()) = bytes_to_string(query);
            }

            switch (opts.src_type) {
                case TabularDataSourceType::k_standard:
                    req.mutable_data_source()->set_type(
                        app::data::v1::TABULAR_DATA_SOURCE_TYPE_STANDARD);
                    break;
                case TabularDataSourceType::k_hot_storage:
                    BOOST_FALLTHROUGH;
                default:
                    req.mutable_data_source()->set_type(
                        app::data::v1::TABULAR_DATA_SOURCE_TYPE_HOT_STORAGE);
                    break;
                case TabularDataSourceType::k_pipeline_sink:
                    req.mutable_data_source()->set_type(
                        app::data::v1::TABULAR_DATA_SOURCE_TYPE_PIPELINE_SINK);
                    break;
            }

            if (opts.pipeline_id.has_value()) {
                req.mutable_data_source()->set_pipeline_id(*opts.pipeline_id);
            }

            if (opts.query_prefix.has_value()) {
                req.set_query_prefix_name(*opts.query_prefix);
            }
        })
        .invoke([](const app::data::v1::TabularDataByMQLResponse& resp) {
            std::vector<DataClient::BSONBytes> result;

            for (const auto& str : resp.raw_data()) {
                result.push_back(string_to_bytes(str));
            }

            return result;
        });
}

std::vector<DataClient::BSONBytes> DataClient::tabular_data_by_mql(
    const std::string& org_id, const std::vector<DataClient::BSONBytes>& mql_binary) {
    return tabular_data_by_mql(org_id, mql_binary, tabular_data_by_mql_opts{});
}

// NEW: AddSequencesToDataset
void DataClient::add_sequences_to_dataset(const std::string& dataset_id,
                                          const std::vector<std::string>& sequence_ids) {
    return pimpl_->client_helper(&DataService::Stub::AddSequencesToDataset)
        .with([&](app::data::v1::AddSequencesToDatasetRequest& req) {
            req.set_dataset_id(dataset_id);
            *req.mutable_sequence_ids() = sdk::impl::to_repeated_field(sequence_ids);
        })
        .invoke();
}

// NEW: RemoveSequencesFromDataset
void DataClient::remove_sequences_from_dataset(const std::string& dataset_id,
                                               const std::vector<std::string>& sequence_ids) {
    return pimpl_->client_helper(&DataService::Stub::RemoveSequencesFromDataset)
        .with([&](app::data::v1::RemoveSequencesFromDatasetRequest& req) {
            req.set_dataset_id(dataset_id);
            *req.mutable_sequence_ids() = sdk::impl::to_repeated_field(sequence_ids);
        })
        .invoke();
}

// NEW: SequencesByDatasetID
std::pair<std::vector<Sequence>, boost::optional<std::string>> DataClient::sequences_by_dataset_id(
    const std::string& dataset_id, const sequences_by_dataset_id_options& opts) {
    return pimpl_->client_helper(&DataService::Stub::SequencesByDatasetID)
        .with([&](app::data::v1::SequencesByDatasetIDRequest& req) {
            req.set_dataset_id(dataset_id);
            if (opts.page_size != 0) {
                req.set_page_size(opts.page_size);
            }
            if (opts.page_token.has_value()) {
                req.set_page_token(*opts.page_token);
            }
        })
        .invoke([](const app::data::v1::SequencesByDatasetIDResponse& resp) {
            std::vector<Sequence> sequences = sdk::impl::from_repeated_field(resp.sequences());
            boost::optional<std::string> next_page_token;
            if (!resp.next_page_token().empty()) {
                next_page_token = resp.next_page_token();
            }
            return std::make_pair(sequences, next_page_token);
        });
}

// NEW: Convenience overload for SequencesByDatasetID with default options.
std::pair<std::vector<Sequence>, boost::optional<std::string>> DataClient::sequences_by_dataset_id(
    const std::string& dataset_id) {
    return sequences_by_dataset_id(dataset_id, sequences_by_dataset_id_options{});
}
}  // namespace sdk
}  // namespace viam