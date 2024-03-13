#pragma once

#include <unordered_map>

#include <boost/variant/get.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/variant.hpp>
#include <google/protobuf/struct.pb.h>

namespace viam {
namespace sdk {

class AttributeMap;

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

class AttributeMap {
   public:
    class value_type;

    using map_type = std::unordered_map<std::string, value_type>;

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

        using base_value_types_ =
            boost::variant<boost::blank, std::string, const char*, int, float, double, bool>;
        using value_types = boost::make_recursive_variant<
            base_value_types_,
            std::unordered_map<std::string, boost::recursive_variant_>,
            std::vector<boost::recursive_variant_>>::type;

        value_type();
        value_type(std::string s);
        value_type(const char* c);
        value_type(int i);
        value_type(float f);
        value_type(double d);
        value_type(bool b);
        value_type(std::vector<value_type> v);
        value_type(map_type);
        value_type(google::protobuf::Value value);

        google::protobuf::Value to_proto() const;

       private:
        value_types value_;
    };

    AttributeMap();
    ~AttributeMap();
    // CR erodkin: add comments to all of this probably
    google::protobuf::Struct to_proto() const;

    bool contains(const std::string& key) const;

    // CR erodkin: consider how to return a ref or pointer to the value type such that we don't have
    // to copy the `value_type`, while retaining guarantees of const-ness.
    value_type get(const std::string& key) const;

    // set a new k/v pair in the AttributeMap
    void set(std::string key, value_type::value_types value) const;
    void set(std::string key, value_type value) const;

    // append a single value to a list type within the AttributeMap
    void append(const std::string& key, value_type::value_types value);
    void append(const std::string& key, value_type value);

    // add a single key/value pair to a map type within the AttributeMap
    void insert(const std::string& key, std::pair<std::string, value_type>);
    void insert(const std::string& key, std::pair<std::string, value_type::value_types>);

    friend bool operator==(const AttributeMap& lhs, const AttributeMap& rhs);
    friend bool operator!(const AttributeMap& v);

    static AttributeMap from_proto(const google::protobuf::Struct& proto);

   private:
    map_type map_;
};

}  // namespace sdk
}  // namespace viam

