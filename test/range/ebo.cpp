/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/range.hpp>

#include <type_traits>
namespace hana = boost::hana;


// This test makes sure that hana::range is empty, which allows
// storage optimizations to take place.

static_assert(std::is_empty<decltype(hana::range_c<int, 0, 10>)>::value, "");
static_assert(std::is_empty<decltype(hana::range_c<long, 5l, 10l>)>::value, "");

// Also make sure that it is EBO-friendly.
template <typename ...T> struct EBO : T... { int value; };
static_assert(sizeof(EBO<decltype(hana::range_c<int, 0, 10>),
                         decltype(hana::range_c<int, 10, 30>)>) == sizeof(int), "");

int main() { }
