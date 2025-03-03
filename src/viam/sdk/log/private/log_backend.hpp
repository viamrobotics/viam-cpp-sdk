#pragma once

#include <boost/log/core/record_view.hpp>
#include <boost/log/sinks/basic_sink_backend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {
namespace impl {

struct LogBackend;

using SinkType = boost::log::sinks::synchronous_sink<LogBackend>;

struct LogBackend : boost::log::sinks::basic_sink_backend<boost::log::sinks::synchronized_feeding> {
    LogBackend(RobotClient* p) : parent(p) {}

    void consume(const boost::log::record_view&);

    static boost::shared_ptr<SinkType> create(RobotClient* p);

    RobotClient* parent;
};

}  // namespace impl
}  // namespace sdk
}  // namespace viam
