#pragma once

#include <unordered_map>

#include <boost/variant/get.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/variant.hpp>
#include <google/protobuf/struct.pb.h>

namespace viam {
namespace sdk {

namespace value_type_details {

template <typename T>
struct get_helper {
    template <typename V>
    static T* get(V& v) {
        return boost::get<T>(&v);
    }

    template <typename V>
    static const T* get(const V& v) {
        return boost::get<T>(&v);
    }
};

}  // namespace value_type_details

class value_type {
   public:
    template <typename T>
    auto get() {
        return value_type_details::get_helper<T>::get(value_);
    }

    template <typename T>
    auto get() const {
        return value_type_details::get_helper<T>::get(value_);
    }

    using map_type = std::unordered_map<std::string, std::unique_ptr<value_type>>;

    using base_value_types_ =
        boost::variant<boost::blank, std::string, const char*, int, float, double, bool>;
    using value_types = boost::make_recursive_variant<
        base_value_types_,
        std::unordered_map<std::string, std::unique_ptr<boost::recursive_variant_>>,
        std::vector<std::unique_ptr<boost::recursive_variant_>>>::type;

    value_type();
    value_type(std::string s);
    value_type(const char* c);
    value_type(int i);
    value_type(float f);
    value_type(double d);
    value_type(bool b);
    value_type(std::vector<std::unique_ptr<value_type>> v);
    value_type(const std::vector<std::unique_ptr<value_type>>& v);
    value_type(std::vector<std::unique_ptr<value_type>>&& v);
    value_type(map_type map);
    value_type(const map_type& map);
    value_type(map_type&& map);
    value_type(google::protobuf::Value value);
    value_type(const google::protobuf::Value& value);
    value_type(google::protobuf::Value&& value);

    google::protobuf::Value to_proto() const;

   private:
    value_types value_;
};

struct attribute_map {
    attribute_map();
    ~attribute_map();
    // CR erodkin: add comments to all of this probably
    google::protobuf::Struct to_proto() const;

    bool contains(const std::string& key) const;

    const value_type& get(const std::string& key) const;
    value_type& get(const std::string& key);

    friend bool operator==(const attribute_map& lhs, const attribute_map& rhs);
    friend bool operator!(const attribute_map& v);

    static attribute_map from_proto(const google::protobuf::Struct& proto);

    value_type::map_type map_;
};

namespace value_type_details {
template <>
struct get_helper<attribute_map> {
    template <typename V>
    static value_type::map_type get(V& v) {
        auto const result = boost::get<value_type::map_type>(&v);
        return result ? *result : boost::blank();
    }

    template <typename V>
    static std::unique_ptr<const std::unique_ptr<value_type>> get(const V& v) {
        auto const result = boost::get<value_type::map_type>(&v);
        return result ? *result : boost::blank();
    }
};
}  // namespace value_type_details

}  // namespace sdk
}  // namespace viam
