/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/concept/metafunction.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


struct x1; struct x2; struct x3;
struct y1 { }; struct y2 { }; struct y3 { };

template <typename F, typename ...T>
constexpr auto valid_call(F f, T ...t) -> decltype(((void)f(t...)), true)
{ return true; }
constexpr auto valid_call(...)
{ return false; }

// metafunction
namespace tc1 {
    template <typename ...> struct f { struct type; };
    using F = decltype(metafunction<f>);
    BOOST_HANA_CONSTANT_CHECK(metafunction<f>() == type_c<f<>::type>);
    BOOST_HANA_CONSTANT_CHECK(metafunction<f>(type_c<x1>) == type_c<f<x1>::type>);
    BOOST_HANA_CONSTANT_CHECK(metafunction<f>(type_c<x1>, type_c<x2>) == type_c<f<x1, x2>::type>);
    BOOST_HANA_CONSTANT_CHECK(metafunction<f>(type_c<x1>, type_c<x2>, type_c<x3>) == type_c<f<x1, x2, x3>::type>);
    static_assert(std::is_same<F::apply<>, f<>>::value, "");
    static_assert(std::is_same<F::apply<x1>, f<x1>>::value, "");
    static_assert(std::is_same<F::apply<x1, x2>, f<x1, x2>>::value, "");
    static_assert(std::is_same<F::apply<x1, x2, x3>, f<x1, x2, x3>>::value, "");

    // Make sure we're SFINAE-friendly
    template <typename ...T> struct no_type { };
    static_assert(!valid_call(metafunction<no_type>), "");
    static_assert(!valid_call(metafunction<no_type>, type_c<x1>), "");

    void fun() {
        // Make sure we don't read from a non-constexpr variable
        auto t = type_c<x1>;
        constexpr auto r = metafunction<f>(t);
        (void)r;
    }

    // Make sure we model the Metafunction concept
    static_assert(Metafunction<decltype(metafunction<f>)>::value, "");
    static_assert(Metafunction<decltype(metafunction<f>)&>::value, "");

    // `metafunction` with non-type arguments
    // 1 arg
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction<f>(y1{}),
        metafunction<f>(type_c<y1>)
    ));

    // 2 args
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction<f>(type_c<x1>, y2{}),
        metafunction<f>(type_c<x1>, type_c<y2>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction<f>(y1{}, type_c<x2>),
        metafunction<f>(type_c<y1>, type_c<x2>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction<f>(y1{}, y2{}),
        metafunction<f>(type_c<y1>, type_c<y2>)
    ));

    // 3 args
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction<f>(type_c<x1>, type_c<x2>, y3{}),
        metafunction<f>(type_c<x1>, type_c<x2>, type_c<y3>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction<f>(type_c<x1>, y2{}, type_c<x3>),
        metafunction<f>(type_c<x1>, type_c<y2>, type_c<x3>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction<f>(type_c<x1>, y2{}, y3{}),
        metafunction<f>(type_c<x1>, type_c<y2>, type_c<y3>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction<f>(y1{}, type_c<x2>, type_c<x3>),
        metafunction<f>(type_c<y1>, type_c<x2>, type_c<x3>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction<f>(y1{}, type_c<x2>, y3{}),
        metafunction<f>(type_c<y1>, type_c<x2>, type_c<y3>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction<f>(y1{}, y2{}, y3{}),
        metafunction<f>(type_c<y1>, type_c<y2>, type_c<y3>)
    ));
}

// metafunction_class
namespace tc2 {
    struct f { template <typename ...> struct apply { struct type; }; };
    using F = decltype(metafunction_class<f>);
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction_class<f>(),
        type_c<f::apply<>::type>
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction_class<f>(type_c<x1>),
        type_c<f::apply<x1>::type>
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction_class<f>(type_c<x1>, type_c<x2>),
        type_c<f::apply<x1, x2>::type>
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction_class<f>(type_c<x1>, type_c<x2>, type_c<x3>),
        type_c<f::apply<x1, x2, x3>::type>
    ));
    static_assert(std::is_same<F::apply<>, f::apply<>>{}, "");
    static_assert(std::is_same<F::apply<x1>, f::apply<x1>>{}, "");
    static_assert(std::is_same<F::apply<x1, x2>, f::apply<x1, x2>>{}, "");
    static_assert(std::is_same<F::apply<x1, x2, x3>, f::apply<x1, x2, x3>>{}, "");

    // Make sure we're SFINAE-friendly
    struct no_type { template <typename ...> struct apply { }; };
    static_assert(!valid_call(metafunction_class<no_type>), "");
    static_assert(!valid_call(metafunction_class<no_type>, type_c<x1>), "");

    void fun() {
        // Make sure we don't read from a non-constexpr variable
        auto t = type_c<x1>;
        constexpr auto r = metafunction_class<f>(t);
        (void)r;
    }

    // Make sure we model the Metafunction concept
    static_assert(Metafunction<decltype(metafunction_class<f>)>::value, "");
    static_assert(Metafunction<decltype(metafunction_class<f>)&>::value, "");

    // `metafunction_class` with non-type arguments
    // 1 arg
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction_class<f>(y1{}),
        metafunction_class<f>(type_c<y1>)
    ));

    // 2 args
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction_class<f>(type_c<x1>, y2{}),
        metafunction_class<f>(type_c<x1>, type_c<y2>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction_class<f>(y1{}, type_c<x2>),
        metafunction_class<f>(type_c<y1>, type_c<x2>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction_class<f>(y1{}, y2{}),
        metafunction_class<f>(type_c<y1>, type_c<y2>)
    ));

    // 3 args
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction_class<f>(type_c<x1>, type_c<x2>, y3{}),
        metafunction_class<f>(type_c<x1>, type_c<x2>, type_c<y3>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction_class<f>(type_c<x1>, y2{}, type_c<x3>),
        metafunction_class<f>(type_c<x1>, type_c<y2>, type_c<x3>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction_class<f>(type_c<x1>, y2{}, y3{}),
        metafunction_class<f>(type_c<x1>, type_c<y2>, type_c<y3>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction_class<f>(y1{}, type_c<x2>, type_c<x3>),
        metafunction_class<f>(type_c<y1>, type_c<x2>, type_c<x3>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction_class<f>(y1{}, type_c<x2>, y3{}),
        metafunction_class<f>(type_c<y1>, type_c<x2>, type_c<y3>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        metafunction_class<f>(y1{}, y2{}, y3{}),
        metafunction_class<f>(type_c<y1>, type_c<y2>, type_c<y3>)
    ));
}

// template_
namespace tc3 {
    template <typename ...> struct f;
    using F = decltype(template_<f>);
    BOOST_HANA_CONSTANT_CHECK(equal(
        template_<f>(),
        type_c<f<>>
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        template_<f>(type_c<x1>),
        type_c<f<x1>>
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        template_<f>(type_c<x1>, type_c<x2>),
        type_c<f<x1, x2>>
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        template_<f>(type_c<x1>, type_c<x2>, type_c<x3>),
        type_c<f<x1, x2, x3>>
    ));
    static_assert(std::is_same<F::apply<>::type, f<>>{}, "");
    static_assert(std::is_same<F::apply<x1>::type, f<x1>>{}, "");
    static_assert(std::is_same<F::apply<x1, x2>::type, f<x1, x2>>{}, "");
    static_assert(std::is_same<F::apply<x1, x2, x3>::type, f<x1, x2, x3>>{}, "");

    // Make sure we model the Metafunction concept
    static_assert(Metafunction<decltype(template_<f>)>::value, "");
    static_assert(Metafunction<decltype(template_<f>)&>::value, "");

    // `template_` with non-type arguments
    // 1 arg
    BOOST_HANA_CONSTANT_CHECK(equal(
        template_<f>(y1{}),
        template_<f>(type_c<y1>)
    ));

    // 2 args
    BOOST_HANA_CONSTANT_CHECK(equal(
        template_<f>(type_c<x1>, y2{}),
        template_<f>(type_c<x1>, type_c<y2>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        template_<f>(y1{}, type_c<x2>),
        template_<f>(type_c<y1>, type_c<x2>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        template_<f>(y1{}, y2{}),
        template_<f>(type_c<y1>, type_c<y2>)
    ));

    // 3 args
    BOOST_HANA_CONSTANT_CHECK(equal(
        template_<f>(type_c<x1>, type_c<x2>, y3{}),
        template_<f>(type_c<x1>, type_c<x2>, type_c<y3>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        template_<f>(type_c<x1>, y2{}, type_c<x3>),
        template_<f>(type_c<x1>, type_c<y2>, type_c<x3>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        template_<f>(type_c<x1>, y2{}, y3{}),
        template_<f>(type_c<x1>, type_c<y2>, type_c<y3>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        template_<f>(y1{}, type_c<x2>, type_c<x3>),
        template_<f>(type_c<y1>, type_c<x2>, type_c<x3>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        template_<f>(y1{}, type_c<x2>, y3{}),
        template_<f>(type_c<y1>, type_c<x2>, type_c<y3>)
    ));
    BOOST_HANA_CONSTANT_CHECK(equal(
        template_<f>(y1{}, y2{}, y3{}),
        template_<f>(type_c<y1>, type_c<y2>, type_c<y3>)
    ));

    // Make sure we can use aliases
    template <typename T> using alias = T;
    static_assert(template_<alias>(type_c<x1>) == type_c<x1>, "");

    void fun() {
        // Make sure we don't read from a non-constexpr variable
        auto t = type_c<x1>;
        constexpr auto r = template_<f>(t);
        (void)r;
    }
}

// trait
namespace tc4 {
    template <typename ...T>
    struct pack_size {
        using type = std::integral_constant<int, sizeof...(T)>;
    };

    // Check that it works, and make sure `trait<F>` returns a hana::integral_constant
    constexpr auto p_0 = trait<pack_size>();
    BOOST_HANA_CONSTANT_CHECK(equal(p_0, integral_c<int, 0>));
    BOOST_HANA_CONSTANT_CHECK(is_a<integral_constant_tag<int>>(p_0));

    constexpr auto p_1 = trait<pack_size>(type_c<x1>);
    BOOST_HANA_CONSTANT_CHECK(equal(p_1, integral_c<int, 1>));
    BOOST_HANA_CONSTANT_CHECK(is_a<integral_constant_tag<int>>(p_1));

    constexpr auto p_2 = trait<pack_size>(type_c<x1>, type_c<x2>);
    BOOST_HANA_CONSTANT_CHECK(equal(p_2, integral_c<int, 2>));
    BOOST_HANA_CONSTANT_CHECK(is_a<integral_constant_tag<int>>(p_2));

    constexpr auto p_3 = trait<pack_size>(type_c<x1>, type_c<x2>, type_c<x3>);
    BOOST_HANA_CONSTANT_CHECK(equal(p_3, integral_c<int, 3>));
    BOOST_HANA_CONSTANT_CHECK(is_a<integral_constant_tag<int>>(p_3));

    // Make sure we don't read from a non-constexpr variable
    void fun() {
        auto t = type_c<x1>;
        constexpr auto r = trait<pack_size>(t);
        (void)r;
    }
}

int main() { }
