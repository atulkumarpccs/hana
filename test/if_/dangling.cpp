// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/if.hpp>

#include <laws/base.hpp>
namespace hana = boost::hana;


template <typename X, typename Y>
decltype(auto) my_or(X&& x, Y&& y) {
    return hana::if_(x, x, static_cast<Y&&>(y));
}

template <typename Q>
decltype(auto) my_implies(Q&& q) {
    return my_or(hana::test::ct_eq<1>{}, static_cast<Q&&>(q));
}

int main() {
    auto eq = hana::test::ct_eq<0>{};
    auto xx = my_implies(eq);
}
