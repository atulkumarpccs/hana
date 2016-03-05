// Copyright Louis Dionne 2013-2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/detail/reverse_indices.hpp>

#include <type_traits>
#include <utility>
namespace hana = boost::hana;


static_assert(std::is_same<
    hana::detail::reverse_indices<std::index_sequence<>>::type,
    std::index_sequence<>
>::value, "");

static_assert(std::is_same<
    hana::detail::reverse_indices<std::index_sequence<0>>::type,
    std::index_sequence<0>
>::value, "");

static_assert(std::is_same<
    hana::detail::reverse_indices<std::index_sequence<0, 1>>::type,
    std::index_sequence<1, 0>
>::value, "");

static_assert(std::is_same<
    hana::detail::reverse_indices<std::index_sequence<0, 1, 2>>::type,
    std::index_sequence<2, 1, 0>
>::value, "");

static_assert(std::is_same<
    hana::detail::reverse_indices<std::index_sequence<0, 1, 2, 3>>::type,
    std::index_sequence<3, 2, 1, 0>
>::value, "");

static_assert(std::is_same<
    hana::detail::reverse_indices<std::index_sequence<0, 1, 2, 3, 4>>::type,
    std::index_sequence<4, 3, 2, 1, 0>
>::value, "");

static_assert(std::is_same<
    hana::detail::reverse_indices<std::index_sequence<0, 1, 2, 3, 4, 5>>::type,
    std::index_sequence<5, 4, 3, 2, 1, 0>
>::value, "");

static_assert(std::is_same<
    hana::detail::reverse_indices<std::index_sequence<0, 1, 2, 3, 4, 5, 6>>::type,
    std::index_sequence<6, 5, 4, 3, 2, 1, 0>
>::value, "");

static_assert(std::is_same<
    hana::detail::reverse_indices<std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7>>::type,
    std::index_sequence<7, 6, 5, 4, 3, 2, 1, 0>
>::value, "");

int main() { }
