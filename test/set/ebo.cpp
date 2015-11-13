/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/set.hpp>
namespace hana = boost::hana;


// Make sure the empty members of a set are reduced to 0 size.
struct A { };
struct B { };

static_assert(sizeof(hana::set<char>) == sizeof(char), "");
static_assert(sizeof(hana::set<A, char>) == sizeof(char), "");
static_assert(sizeof(hana::set<char, A>) == sizeof(char), "");

static_assert(sizeof(hana::set<A, char, B>) == sizeof(char), "");
static_assert(sizeof(hana::set<A, B, char>) == sizeof(char), "");
static_assert(sizeof(hana::set<char, A, B>) == sizeof(char), "");


// Also make sure that set itself is EBO-friendly
template <typename ...T> struct EBO : T... { char weight; };
static_assert(sizeof(EBO<hana::set<A>, hana::set<B>>) ==
              sizeof(EBO<hana::set<A>>), "");

int main() { }
