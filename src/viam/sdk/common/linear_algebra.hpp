#pragma once

#include <array>

#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_traits.hpp>

#include <viam/sdk/common/proto_convert.hpp>

namespace viam {
namespace common {
namespace v1 {

class Vector3;
}
}  // namespace common
}  // namespace viam

namespace viam {
namespace sdk {

// In the future, we may wish to inline this whole class
// for performance reasons.

struct Vector3 {
    using scalar_type = double;

    scalar_type x() const;
    scalar_type y() const;
    scalar_type z() const;
    /// Set the x value of the vector (can be chained)
    Vector3& set_x(scalar_type x);
    /// Set the y value of the vector (can be chained)
    Vector3& set_y(scalar_type y);
    /// Set the z value of the vector (can be chained)
    Vector3& set_z(scalar_type z);

    std::array<scalar_type, 3> data;
};

namespace proto_convert_details {

template <>
struct to_proto<Vector3> {
    void operator()(const Vector3&, common::v1::Vector3*) const;
};

template <>
struct from_proto<common::v1::Vector3> {
    Vector3 operator()(const common::v1::Vector3*) const;
};

}  // namespace proto_convert_details
}  // namespace sdk
}  // namespace viam

namespace boost {
namespace qvm {

template <>
struct vec_traits<viam::sdk::Vector3> {
    static int const dim = 3;
    using vec_type = ::viam::sdk::Vector3;
    using scalar_type = vec_type::scalar_type;

    template <int I>
    static inline scalar_type& write_element(vec_type& v) {
        return v.data[I];
    }

    template <int I>
    static inline scalar_type read_element(vec_type const& v) {
        return v.data[I];
    }

    static inline scalar_type& write_element_idx(int i, vec_type& v) {
        return v.data[i];
    }

    static inline scalar_type read_element_idx(int i, vec_type const& v) {
        return v.data[i];
    }
};

}  // namespace qvm
}  // namespace boost
