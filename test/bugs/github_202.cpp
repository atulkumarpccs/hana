/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/basic_tuple.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/set.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <utility>
namespace hana = boost::hana;


//
// This test makes sure that empty members of Hana's containers are reduced
// to 0 size, even when those empty members are other containers or library
// types. This is to check for https://github.com/boostorg/hana/issues/202,
// where we realized that some types in Hana shared common bases, and that
// would cause EBO not to fire, resulting in bloat.
//

struct NonEmpty : hana::type<NonEmpty> {
    int weight;
};

template <typename Empty>
void test() {
    //////////////////////////////////
    // Test for hana::map
    //////////////////////////////////
    using Map1 = decltype(hana::make_map(
        hana::make_pair(std::declval<Empty>(), NonEmpty{})
    ));
    static_assert(sizeof(Map1) == sizeof(NonEmpty), "");

    using Map2 = decltype(hana::make_map(
        hana::make_pair(std::declval<Empty>(), NonEmpty{}),
        hana::make_pair(hana::type_c<struct Key>, hana::type_c<struct Value>)
    ));
    static_assert(sizeof(Map2) == sizeof(NonEmpty), "");

    //////////////////////////////////
    // Test for hana::pair
    //////////////////////////////////
    static_assert(sizeof(hana::pair<int, Empty>) == sizeof(int), "");
    static_assert(sizeof(hana::pair<Empty, int>) == sizeof(int), "");

    //////////////////////////////////
    // Test for hana::set
    //////////////////////////////////
    using Set1 = decltype(hana::make_set(std::declval<Empty>(), NonEmpty{}));
    static_assert(sizeof(Set1) == sizeof(NonEmpty), "");

    using Set2 = decltype(hana::make_set(NonEmpty{}, std::declval<Empty>()));
    static_assert(sizeof(Set2) == sizeof(NonEmpty), "");

    //////////////////////////////////
    // Test for hana::tuple
    //////////////////////////////////
    struct Other { };
    static_assert(sizeof(hana::tuple<int>) == sizeof(int), "");

    static_assert(sizeof(hana::tuple<int, Empty>) == sizeof(int), "");
    static_assert(sizeof(hana::tuple<Empty, int>) == sizeof(int), "");

    static_assert(sizeof(hana::tuple<Empty, int, Other>) == sizeof(int), "");
    static_assert(sizeof(hana::tuple<Other, int, Empty>) == sizeof(int), "");
    static_assert(sizeof(hana::tuple<Empty, Other, int>) == sizeof(int), "");
    static_assert(sizeof(hana::tuple<Other, Empty, int>) == sizeof(int), "");
    static_assert(sizeof(hana::tuple<int, Empty, Other>) == sizeof(int), "");
    static_assert(sizeof(hana::tuple<int, Other, Empty>) == sizeof(int), "");


    //////////////////////////////////
    // Test for hana::basic_tuple
    //////////////////////////////////
    static_assert(sizeof(hana::basic_tuple<int>) == sizeof(int), "");

    static_assert(sizeof(hana::basic_tuple<int, Empty>) == sizeof(int), "");
    static_assert(sizeof(hana::basic_tuple<Empty, int>) == sizeof(int), "");

    static_assert(sizeof(hana::basic_tuple<Empty, int, Other>) == sizeof(int), "");
    static_assert(sizeof(hana::basic_tuple<Other, int, Empty>) == sizeof(int), "");
    static_assert(sizeof(hana::basic_tuple<Empty, Other, int>) == sizeof(int), "");
    static_assert(sizeof(hana::basic_tuple<Other, Empty, int>) == sizeof(int), "");
    static_assert(sizeof(hana::basic_tuple<int, Empty, Other>) == sizeof(int), "");
    static_assert(sizeof(hana::basic_tuple<int, Other, Empty>) == sizeof(int), "");
}

int main() {
    test<hana::type<struct T>>();
    test<hana::char_<'x'>>();
    test<hana::pair<hana::type<struct T>, hana::char_<'x'>>>();
    test<decltype(hana::range_c<int, 0, 10>)>();
    test<decltype(hana::string_c<'x', 'y', 'z'>)>();
    test<hana::tuple<hana::char_<'x'>, hana::type<struct T>>>();
    test<decltype(hana::make_map(hana::make_pair(hana::type_c<struct T>, hana::int_c<3>)))>();
    test<decltype(hana::make_set(hana::type_c<struct T>))>();
}
