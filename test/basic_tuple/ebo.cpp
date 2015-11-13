/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/basic_tuple.hpp>
namespace hana = boost::hana;


// Make sure the empty members of a basic_tuple are reduced to 0 size.
struct A { };
struct B { };

static_assert(sizeof(hana::basic_tuple<char>) == sizeof(char), "");
static_assert(sizeof(hana::basic_tuple<A, char>) == sizeof(char), "");
static_assert(sizeof(hana::basic_tuple<char, A>) == sizeof(char), "");

static_assert(sizeof(hana::basic_tuple<A, char, B>) == sizeof(char), "");
static_assert(sizeof(hana::basic_tuple<A, B, char>) == sizeof(char), "");
static_assert(sizeof(hana::basic_tuple<char, A, B>) == sizeof(char), "");


// Also make sure that basic_tuple itself is EBO-friendly
template <typename ...T> struct EBO : T... { char weight; };
static_assert(sizeof(EBO<hana::basic_tuple<A>, hana::basic_tuple<B>>) == sizeof(char), "");


int main() { }
