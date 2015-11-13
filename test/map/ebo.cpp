/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/map.hpp>
namespace hana = boost::hana;


// Make sure the empty members of a map are reduced to 0 size.
struct A { };
struct B { };

static_assert(sizeof(hana::map<char>) == sizeof(char), "");
static_assert(sizeof(hana::map<A, char>) == sizeof(char), "");
static_assert(sizeof(hana::map<char, A>) == sizeof(char), "");

static_assert(sizeof(hana::map<A, char, B>) == sizeof(char), "");
static_assert(sizeof(hana::map<A, B, char>) == sizeof(char), "");
static_assert(sizeof(hana::map<char, A, B>) == sizeof(char), "");


// Also make sure that basic_tuple itself is EBO-friendly.
template <typename ...T> struct EBO : T... { char weight; };
static_assert(sizeof(EBO<hana::map<A>, hana::map<B>>) ==
              sizeof(EBO<hana::map<A>>), "");

int main() { }
