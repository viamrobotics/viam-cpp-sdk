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

std::string DataClient::CreateSequence(
    const std::string& part_id,
    const std::vector<DataClient::SequenceResourceFilter>& resources,
    const std::vector<std::string>& sequence_tags,
    const boost::optional<std::chrono::system_clock::time_point>& start_time,
    const boost::optional<std::chrono::system_clock::time_point>& end_time) {
    return pimpl_->client_helper(&DataService::Stub::CreateSequence)
        .with([&](app::data::v1::CreateSequenceRequest& req) {
            req.set_part_id(part_id);
            for (const auto& r : resources) {
                *req.mutable_resources()->Add() = to_proto(r);
            }
            for (const auto& tag : sequence_tags) {
                *req.mutable_sequence_tags()->Add() = tag;
            }
            if (start_time.has_value()) {
                *req.mutable_start_time() = to_proto(*start_time);
            }
            if (end_time.has_value()) {
                *req.mutable_end_time() = to_proto(*end_time);
            }
        })
        .invoke([](const app::data::v1::CreateSequenceResponse& resp) { return resp.id(); });
}

std::string DataClient::CreateSequence(
    const std::string& part_id,
    const std::vector<DataClient::SequenceResourceFilter>& resources,
    const std::vector<std::string>& sequence_tags) {
    return CreateSequence(part_id, resources, sequence_tags, boost::none, boost::none);
}

DataClient::Sequence DataClient::GetSequence(const std::string& id) {
    return pimpl_->client_helper(&DataService::Stub::GetSequence)
        .with([&](app::data::v1::GetSequenceRequest& req) { req.set_id(id); })
        .invoke([](const app::data::v1::GetSequenceResponse& resp) { return from_proto(resp.sequence()); });
}

void DataClient::UpdateSequence(const std::string& id, const update_sequence_options& opts) {
    return pimpl_->client_helper(&DataService::Stub::UpdateSequence)
        .with([&](app::data::v1::UpdateSequenceRequest& req) {
            req.set_id(id);
            if (opts.resources.has_value()) {
                for (const auto& r : *opts.resources) {
                    *req.mutable_resources()->Add() = to_proto(r);
                }
            }
            if (opts.sequence_tags.has_value()) {
                for (const auto& tag : *opts.sequence_tags) {
                    *req.mutable_sequence_tags()->Add() = tag;
                }
            }
            if (opts.start_time.has_value()) {
                *req.mutable_start_time() = to_proto(*opts.start_time);
            }
            if (opts.end_time.has_value()) {
                *req.mutable_end_time() = to_proto(*opts.end_time);
            }
            for (const auto& path : opts.field_mask) {
                *req.mutable_field_mask()->mutable_paths()->Add() = path;
            }
        })
        .invoke([](const app::data::v1::UpdateSequenceResponse& resp) {});
}

void DataClient::DeleteSequence(const std::string& id) {
    return pimpl_->client_helper(&DataService::Stub::DeleteSequence)
        .with([&](app::data::v1::DeleteSequenceRequest& req) { req.set_id(id); })
        .invoke([](const app::data::v1::DeleteSequenceResponse& resp) {});
}

std::pair<std::vector<DataClient::Sequence>, std::string>
DataClient::ListSequences(const std::string& organization_id, const list_sequences_options& opts) {
    return pimpl_->client_helper(&DataService::Stub::ListSequences)
        .with([&](app::data::v1::ListSequencesRequest& req) {
            req.set_organization_id(organization_id);
            if (opts.page_token.has_value()) {
                req.set_page_token(*opts.page_token);
            }
            if (opts.page_size.has_value()) {
                req.set_page_size(*opts.page_size);
            }
        })
        .invoke([](const app::data::v1::ListSequencesResponse& resp) {
            return std::make_pair(from_repeated_field(resp.sequences()), resp.next_page_token());
        });
}

std::pair<std::vector<DataClient::Sequence>, std::string>
DataClient::ListSequences(const std::string& organization_id) {
    return ListSequences(organization_id, list_sequences_options{});
}
}  // namespace sdk
}  // namespace viam