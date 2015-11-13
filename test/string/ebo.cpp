/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/string.hpp>

#include <type_traits>
namespace hana = boost::hana;


// This test makes sure that hana::string is empty, which allows
// storage optimizations to take place.

static_assert(std::is_empty<hana::string<'a', 'b', 'c'>>::value, "");

// Also make sure that it is EBO-friendly.
template <typename ...T> struct EBO : T... { int value; };
static_assert(sizeof(EBO<hana::string<'a', 'b'>, hana::string<'c', 'd'>>) == sizeof(int), "");

int main() { }
