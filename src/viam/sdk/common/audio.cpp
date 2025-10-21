#include <viam/sdk/common/audio.hpp>

namespace viam {
namespace sdk {

bool operator==(const properties& lhs, const properties& rhs) {
    return lhs.supported_codecs == rhs.supported_codecs &&
           lhs.sample_rate_hz == rhs.sample_rate_hz &&
           lhs.num_channels == rhs.num_channels;
}

}  // namespace sdk
}  // namespace viam

