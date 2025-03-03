#include <viam/sdk/log/private/log_backend.hpp>

namespace viam {
namespace sdk {
namespace impl {

void LogBackend::consume(const boost::log::record_view& rec) {}

boost::shared_ptr<SinkType> LogBackend::create(RobotClient* p) {
    auto backend = boost::make_shared<LogBackend>(p);
    return boost::make_shared<SinkType>(backend);
}

}  // namespace impl
}  // namespace sdk
}  // namespace viam
