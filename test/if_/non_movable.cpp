// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/eval_if.hpp>
#include <boost/hana/fwd/not.hpp>
#include <boost/hana/fwd/while.hpp>
#include <boost/hana/if.hpp>
namespace hana = boost::hana;


// This test makes sure that if_ can be used with non-movable branches.

template <bool Value>
struct Boolean { };

namespace boost { namespace hana {
    template <bool Value>
    struct while_impl<Boolean<Value>> {
        // Not implemented
    };

    template <bool Value>
    struct not_impl<Boolean<Value>> {
        // Not implemented
    };

    template <bool Value>
    struct eval_if_impl<Boolean<Value>> {
        template <typename Cond, typename Then, typename Else>
        static constexpr decltype(auto) apply(Cond const&, Then&& t, Else&& e) {
            return hana::eval_if(hana::bool_c<Value>, static_cast<Then&&>(t),
                                                      static_cast<Else&&>(e));
        }
    };
}}

template <int v>
struct NonMovable {
    NonMovable() = default;
    NonMovable(NonMovable const&) = delete;
    NonMovable(NonMovable&&) = delete;

    NonMovable& operator=(NonMovable const&) = delete;
    NonMovable& operator=(NonMovable&&) = delete;
};

static_assert(std::is_same<
    decltype(hana::if_(Boolean<true>{}, NonMovable<3>{}, NonMovable<4>{})),
    NonMovable<3>&&
>{}, "");

static_assert(std::is_same<
    decltype(hana::if_(Boolean<false>{}, NonMovable<3>{}, NonMovable<4>{})),
    NonMovable<4>&&
>{}, "");

int main() {
    NonMovable<3> a;
    NonMovable<4> b;
    BOOST_HANA_RUNTIME_CHECK(&hana::if_(Boolean<true>{}, a, b) == &a);
    BOOST_HANA_RUNTIME_CHECK(&hana::if_(Boolean<false>{}, a, b) == &b);
}
