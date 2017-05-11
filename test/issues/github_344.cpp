// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>

#include <type_traits>
namespace hana = boost::hana;


struct Trivial { int i; };

// pair
static_assert(std::is_trivially_copy_constructible<hana::pair<Trivial, Trivial>>::value, "");
static_assert(std::is_trivially_move_constructible<hana::pair<Trivial, Trivial>>::value, "");

// tuple
static_assert(std::is_trivially_copy_constructible<hana::tuple<>>::value, "");
static_assert(std::is_trivially_move_constructible<hana::tuple<>>::value, "");
static_assert(std::is_trivially_copy_constructible<hana::tuple<Trivial>>::value, "");
static_assert(std::is_trivially_move_constructible<hana::tuple<Trivial>>::value, "");
static_assert(std::is_trivially_copy_constructible<hana::tuple<Trivial, Trivial>>::value, "");
static_assert(std::is_trivially_move_constructible<hana::tuple<Trivial, Trivial>>::value, "");

int main() { }
