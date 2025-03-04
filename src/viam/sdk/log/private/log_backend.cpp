#include <viam/sdk/log/private/log_backend.hpp>

#include <boost/smart_ptr/make_shared.hpp>

#include <viam/sdk/log/private/keywords.hpp>

namespace viam {
namespace sdk {
namespace impl {

void LogBackend::consume(const boost::log::record_view& rec) {
    // auto time = *rec[attr_time];

    parent->log(
        *rec[attr_channel], to_string(*rec[attr_sev]), *rec[boost::log::expressions::smessage], {});
}

boost::shared_ptr<SinkType> LogBackend::create(RobotClient* p) {
    auto backend = boost::make_shared<LogBackend>(p);
    return boost::make_shared<SinkType>(backend);
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
