#pragma once
#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_traits.hpp>

#include <viam/api/common/v1/common.pb.h>
namespace viam {
namespace sdk {

// In the future, we may wish to inline this whole class
// for performance reasons.

class Vector3 {
   public:
    using scalar_type = double;
    Vector3(scalar_type x, scalar_type y, scalar_type z);
    Vector3();

    scalar_type x() const;
    scalar_type y() const;
    scalar_type z() const;
    /// Set the x value of the vector (can be chained)
    Vector3& set_x(scalar_type x);
    /// Set the y value of the vector (can be chained)
    Vector3& set_y(scalar_type y);
    /// Set the z value of the vector (can be chained)
    Vector3& set_z(scalar_type z);

    const std::array<scalar_type, 3>& data() const;
    std::array<scalar_type, 3>& data();
    static viam::common::v1::Vector3 to_proto(const Vector3& vec);
    static Vector3 from_proto(viam::common::v1::Vector3 vec);

   private:
    std::array<scalar_type, 3> data_;
};

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
        return v.data()[I];
    }

    template <int I>
    static inline scalar_type read_element(vec_type const& v) {
        return v.data()[I];
    }

    static inline scalar_type& write_element_idx(int i, vec_type& v) {
        return v.data()[i];
    }

    static inline scalar_type read_element_idx(int i, vec_type const& v) {
        return v.data()[i];
    }
};

}  // namespace qvm
}  // namespace boost
