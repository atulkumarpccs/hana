/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral_constant.hpp>

#include <type_traits>
namespace hana = boost::hana;


// This test makes sure that integral_constant is empty, which allows
// storage optimizations to take place.

static_assert(std::is_empty<hana::int_<3>>::value, "");
static_assert(std::is_empty<hana::char_<'x'>>::value, "");
static_assert(std::is_empty<hana::long_<4l>>::value, "");


// Also make sure that it is EBO-friendly.
template <typename ...T> struct EBO : T... { int value; };
static_assert(sizeof(EBO<hana::char_<'x'>, hana::char_<'y'>>) == sizeof(int), "");

int main() { }
