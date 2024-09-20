#define BOOST_TEST_MODULE test module test_proto_value_visit

#include <viam/sdk/common/proto_value_visit.hpp>

#include <tuple>
#include <type_traits>

#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/list.hpp>
#include <boost/mp11/tuple.hpp>
#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/common/proto_value.hpp>

namespace viam {
namespace sdktests {

using namespace viam::sdk;
using namespace boost::mp11;

BOOST_AUTO_TEST_SUITE(test_proto_value_visit)

template <typename T>
using KindType = typename proto_value_details::kind<T>::type;

struct const_visitor {
    int kind;

    template <typename T, typename = KindType<T>>
    bool operator()(const T&) const {
        return kind == KindType<T>{};
    }

    bool operator()(...) const {
        return false;
    }

    template <typename T>
    static void test(int kind_, T t) {
        const ProtoValue v(t);
        BOOST_TEST(visit(const_visitor{kind_}, v), "const_visitor with kind " << kind_);
    }
};

struct mutable_visitor {
    int kind;

    template <typename T, typename = KindType<T>>
    bool operator()(T&) {
        return kind == KindType<T>{};
    }

    bool operator()(...) {
        return false;
    }

    template <typename T>
    static void test(int kind_, T t) {
        ProtoValue v(t);
        BOOST_TEST(visit(mutable_visitor{kind_}, v), "mutable_visitor with kind " << kind_);
    }
};

struct by_value_visitor {
    int kind;

    template <typename T, typename = KindType<T>>
    bool operator()(T) && {
        return kind == KindType<T>{};
    }

    bool operator()(...) && {
        return false;
    }

    template <typename T>
    static void test(int kind_, T t) {
        ProtoValue v(t);
        by_value_visitor visitor{kind_};

        BOOST_TEST(visit(std::move(visitor), std::move(v)), "by_value_visitor with kind " << kind_);
    }
};

struct rvalue_visitor {
    // Note that in
    // template<typename T> bool operator()(T&&);
    // we are in a deduced context, so T&& is a forwarding reference rather than an rvalue
    // reference. Thus we spell out the overloads.

    bool operator()(std::nullptr_t&&) && {
        return kind == ProtoValue::Kind::null;
    }
    bool operator()(bool&&) && {
        return kind == ProtoValue::Kind::bool_;
    }

    bool operator()(int&&) && {
        return kind == ProtoValue::Kind::int_;
    }

    bool operator()(double&&) && {
        return kind == ProtoValue::Kind::double_;
    }

    bool operator()(std::string&&) && {
        return kind == ProtoValue::Kind::string;
    }

    bool operator()(ProtoList&&) && {
        return kind == ProtoValue::Kind::list;
    }

    bool operator()(ProtoStruct&&) && {
        return kind == ProtoValue::Kind::struct_;
    }

    bool operator()(...) && {
        return false;
    }
    int kind;

    template <typename T>
    static void test(int kind_, T t) {
        ProtoValue v(t);
        rvalue_visitor visitor{kind_};

        BOOST_TEST(visit(std::move(visitor), std::move(v)), "rvalue visitor with kind " << kind_);
    }
};

BOOST_AUTO_TEST_CASE(test_visitor) {
    auto test_cases = std::make_tuple(
        std::make_pair(ProtoValue::Kind::null, nullptr),
        std::make_pair(ProtoValue::Kind::bool_, true),
        std::make_pair(ProtoValue::Kind::int_, 5),
        std::make_pair(ProtoValue::Kind::double_, 12.345),
        std::make_pair(ProtoValue::Kind::string, "meow"),
        std::make_pair(ProtoValue::Kind::list, ProtoList({{ProtoValue(1), ProtoValue("woof")}})),
        std::make_pair(ProtoValue::Kind::struct_, ProtoStruct({{"string", "str"}, {"int", 5}})));

    using visitors = mp_list<const_visitor, mutable_visitor, by_value_visitor, rvalue_visitor>;

    mp_for_each<visitors>([&](auto visitor) {
        using Visitor = decltype(visitor);
        tuple_for_each(test_cases,
                       [](auto test_pair) { Visitor::test(test_pair.first, test_pair.second); });
    });

    // This is more a compilation test; see remark in proto_value_visit.hpp.
    tuple_for_each(test_cases, [](auto test_pair) {
        ProtoValue val(test_pair.second);
        BOOST_TEST(visit([](auto&) { return true; }, val));
    });

    tuple_for_each(test_cases, [](auto test_pair) {
        auto visitor = [](auto&&) { return true; };
        const ProtoValue const_val(test_pair.second);
        ProtoValue mut_val(test_pair.second);

        BOOST_TEST(visit(visitor, const_val));
        BOOST_TEST(visit(visitor, std::move(mut_val)));
    });
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace sdktests
}  // namespace viam
