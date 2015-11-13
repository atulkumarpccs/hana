/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/pair.hpp>
namespace hana = boost::hana;


// Make sure the empty members of a pair are reduced to 0 size.
struct A { };
struct B { };

static_assert(sizeof(hana::pair<A, char>) == sizeof(char), "");
static_assert(sizeof(hana::pair<char, A>) == sizeof(char), "");
static_assert(sizeof(hana::pair<A, B>) == 1, "");


// Also make sure that pair itself is EBO-friendly
struct C { };
struct D { };

template <typename ...T> struct EBO : T... { char weight; };
static_assert(sizeof(EBO<hana::pair<A, B>, hana::pair<C, D>>) ==
              sizeof(EBO<hana::pair<A, B>>), "");


int main() { }
