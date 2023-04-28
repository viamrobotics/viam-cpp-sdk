#pragma once
#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_traits.hpp>

#include <viam/api/common/v1/common.pb.h>
namespace viam {
namespace sdk {

class Vector3 {
   public:
    Vector3(double x, double y, double z) : data_{x, y, z} {};
    Vector3() : Vector3(0.0, 0.0, 0.0){};

    double x() const;
    double y() const;
    double z() const;
    void set_x(double x);
    void set_y(double y);
    void set_z(double z);
    std::array<double, 3> data() const;
    std::array<double, 3>& mutable_data();
    static viam::common::v1::Vector3 to_proto(const Vector3& vec);
    static Vector3 from_proto(viam::common::v1::Vector3 vec);

   private:
    std::array<double, 3> data_;
};

}  // namespace sdk
}  // namespace viam

namespace boost {
namespace qvm {

template <>
struct vec_traits<viam::sdk::Vector3> {
    static int const dim = 3;
    typedef double scalar_type;

    template <int I>
    static inline scalar_type& write_element(viam::sdk::Vector3& v) {
        return v.mutable_data()[I];
    }

    template <int I>
    static inline scalar_type read_element(viam::sdk::Vector3 const& v) {
        return v.data()[I];
    }

    static inline scalar_type& write_element_idx(int i, viam::sdk::Vector3& v) {
        return v.mutable_data()[i];
    }

    static inline scalar_type read_element_idx(int i, viam::sdk::Vector3 const& v) {
        return v.data()[i];
    }
};

}  // namespace qvm
}  // namespace boost
