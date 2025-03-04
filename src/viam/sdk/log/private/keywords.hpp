#pragma once

#include <boost/log/attributes/clock.hpp>
#include <boost/log/expressions/keyword.hpp>

namespace viam {
namespace sdk {

BOOST_LOG_ATTRIBUTE_KEYWORD(attr_channel, "Channel", std::string);
BOOST_LOG_ATTRIBUTE_KEYWORD(attr_sev, "Severity", viam::sdk::log_level);
BOOST_LOG_ATTRIBUTE_KEYWORD(attr_file, "file", const char*);
BOOST_LOG_ATTRIBUTE_KEYWORD(attr_line, "line", unsigned int);
BOOST_LOG_ATTRIBUTE_KEYWORD(attr_time,
                            "TimeStamp",
                            boost::log::attributes::local_clock::value_type);

}  // namespace sdk
}  // namespace viam
