/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


// Make sure the empty members of a tuple are reduced to 0 size.
struct A { };
struct B { };

static_assert(sizeof(hana::tuple<char>) == sizeof(char), "");
static_assert(sizeof(hana::tuple<A, char>) == sizeof(char), "");
static_assert(sizeof(hana::tuple<char, A>) == sizeof(char), "");

static_assert(sizeof(hana::tuple<A, char, B>) == sizeof(char), "");
static_assert(sizeof(hana::tuple<A, B, char>) == sizeof(char), "");
static_assert(sizeof(hana::tuple<char, A, B>) == sizeof(char), "");


// Also make sure that tuple itself is EBO-friendly
template <typename ...T> struct EBO : T... { char weight; };
static_assert(sizeof(EBO<hana::tuple<A>, hana::tuple<B>>) ==
              sizeof(EBO<hana::tuple<A>>), "");


int main() { }
