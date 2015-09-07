/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/basic_set.hpp>
#include <boost/hana/contains.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/type.hpp>
namespace hana = boost::hana;


constexpr hana::basic_set<hana::type<int>, hana::basic_type<char>, hana::int_<3>> xs{};

BOOST_HANA_CONSTANT_CHECK(hana::contains(xs, hana::type<int>{}));
BOOST_HANA_CONSTANT_CHECK(hana::contains(xs, hana::type<char>{}));
BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::contains(xs, hana::type<void>{})));

BOOST_HANA_CONSTANT_CHECK(hana::contains(xs, hana::int_<3>{}));
BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::contains(xs, hana::int_<4>{})));

int main() { }
