#include <viam/sdk/app/data_client.hpp>

#include <boost/config.hpp>

#include <grpcpp/channel.h>

#include <viam/api/app/data/v1/data.grpc.pb.h>
#include <viam/api/app/data/v1/data.pb.h>

#include <viam/sdk/common/client_helper.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/app/data_client_proto_conversions.hpp>

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

std::string DataClient::create_sequence(const std::string& org_id, const CreateSequenceOptions& opts) {
    return pimpl_->client_helper(&DataService::Stub::CreateSequence)
        .with([&](app::data::v1::CreateSequenceRequest& req) {
            req.set_organization_id(org_id);
            for (const auto& resource : opts.resources) {
                *req.add_resources() = to_proto(resource);
            }
            for (const auto& tag : opts.sequence_tags) {
                req.add_sequence_tags(tag);
            }
            if (opts.start_time.has_value()) {
                *req.mutable_start_time() = to_proto(*opts.start_time);
            }
            if (opts.end_time.has_value()) {
                *req.mutable_end_time() = to_proto(*opts.end_time);
            }
        })
        .invoke([](const app::data::v1::CreateSequenceResponse& resp) { return resp.id(); });
}

Sequence DataClient::get_sequence(const GetSequenceOptions& opts) {
    return pimpl_->client_helper(&DataService::Stub::GetSequence)
        .with([&](app::data::v1::GetSequenceRequest& req) { req.set_id(opts.id); })
        .invoke([](const app::data::v1::GetSequenceResponse& resp) { return from_proto(resp.sequence()); });
}

void DataClient::update_sequence(const UpdateSequenceOptions& opts) {
    pimpl_->client_helper(&DataService::Stub::UpdateSequence)
        .with([&](app::data::v1::UpdateSequenceRequest& req) {
            req.set_id(opts.id);
            if (opts.resources.has_value()) {
                for (const auto& resource : *opts.resources) {
                    *req.add_resources() = to_proto(resource);
                }
            }
            if (opts.sequence_tags.has_value()) {
                for (const auto& tag : *opts.sequence_tags) {
                    req.add_sequence_tags(tag);
                }
            }
            if (opts.start_time.has_value()) {
                *req.mutable_start_time() = to_proto(*opts.start_time);
            }
            if (opts.end_time.has_value()) {
                *req.mutable_end_time() = to_proto(*opts.end_time);
            }
            if (opts.field_mask_paths.has_value()) {
                *req.mutable_field_mask() = to_proto(*opts.field_mask_paths);
            }
        })
        .invoke([](const app::data::v1::UpdateSequenceResponse& resp) {});
}

void DataClient::delete_sequence(const DeleteSequenceOptions& opts) {
    pimpl_->client_helper(&DataService::Stub::DeleteSequence)
        .with([&](app::data::v1::DeleteSequenceRequest& req) { req.set_id(opts.id); })
        .invoke([](const app::data::v1::DeleteSequenceResponse& resp) {});
}

std::pair<std::vector<Sequence>, std::string> DataClient::list_sequences(
    const std::string& org_id, const ListSequencesOptions& opts) {
    return pimpl_->client_helper(&DataService::Stub::ListSequences)
        .with([&](app::data::v1::ListSequencesRequest& req) {
            req.set_organization_id(org_id);
            if (opts.page_token.has_value()) {
                req.set_page_token(*opts.page_token);
            }
            if (opts.page_size.has_value()) {
                req.set_page_size(*opts.page_size);
            }
        })
        .invoke([](const app::data::v1::ListSequencesResponse& resp) {
            std::vector<Sequence> sequences;
            for (const auto& sequence_proto : resp.sequences()) {
                sequences.push_back(from_proto(sequence_proto));
            }
            return std::make_pair(sequences, resp.next_page_token());
        });
}

std::pair<std::vector<Sequence>, std::string> DataClient::list_sequences(const std::string& org_id) {
    return list_sequences(org_id, ListSequencesOptions{});
}
}  // namespace sdk
}  // namespace viam