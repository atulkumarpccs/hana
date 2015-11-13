/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <type_traits>
namespace hana = boost::hana;


// This test makes sure that hana::type is empty, which allows
// storage optimizations to take place.

static_assert(std::is_empty<hana::type<struct T>>::value, "");
static_assert(std::is_empty<hana::basic_type<struct T>>::value, "");

// Also make sure that it is EBO-friendly.
template <typename ...T> struct EBO : T... { int value; };
static_assert(sizeof(EBO<hana::type<struct T>, hana::type<struct U>>) == sizeof(int), "");

int main() { }
