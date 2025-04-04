#include <viam/sdk/log/private/log_backend.hpp>

#include <boost/date_time/posix_time/conversion.hpp>
#include <boost/smart_ptr/make_shared.hpp>

namespace viam {
namespace sdk {
namespace impl {

time_pt ptime_convert(const boost::posix_time::ptime& from) {
    namespace posix_time = boost::posix_time;

    posix_time::time_duration const time_since_epoch = from - posix_time::from_time_t(0);
    const time_pt t = std::chrono::system_clock::from_time_t(time_since_epoch.total_seconds());
    const long nsec = time_since_epoch.fractional_seconds() *
                      (1000000000 / posix_time::time_duration::ticks_per_second());
    return t + std::chrono::nanoseconds(nsec);
}

void LogBackend::consume(const boost::log::record_view& rec) const {
    parent->log(*rec[attr_channel_type{}],
                to_string(*rec[attr_sev_type{}]),
                *rec[boost::log::expressions::smessage],
                ptime_convert(*rec[attr_time_type{}]));
}

boost::shared_ptr<SinkType> LogBackend::create(RobotClient* p) {
    auto backend = boost::make_shared<LogBackend>(p);
    return boost::make_shared<SinkType>(backend);
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
