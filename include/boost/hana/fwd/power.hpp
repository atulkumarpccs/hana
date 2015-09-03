/*!
@file
Forward declares `boost::hana::power`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_POWER_HPP
#define BOOST_HANA_FWD_POWER_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Elevate a ring element to its `n`th power.
    //! @relates Ring
    //!
    //! Specifically, `power(x, n)`, is equivalent to multiplying `x` with
    //! itself `n` times using the Ring's multiplication. If the power is
    //! equal to `zero`, the Ring's identity (`one`) is returned.
    //!
    //! @param x
    //! A `Ring` element that is elevated to its `n`th power.
    //!
    //! @param n
    //! A `Constant` of an unsigned integral type representing the power
    //! to which `x` is elevated.
    //!
    //!
    //! @note
    //! Only the tag of `x` is used for tag-dispatching.
    //!
    //! Example
    //! -------
    //! @include example/power.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto power = [](auto&& x, auto&& n) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename R, typename = void>
    struct power_impl : power_impl<R, when<true>> { };

    struct power_t {
        template <typename X, typename N>
        constexpr decltype(auto) operator()(X&& x, N&& n) const;
    };

    constexpr power_t power{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_POWER_HPP
