/*!
@file
Forward declares `boost::hana::drop_front`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_DROP_FRONT_HPP
#define BOOST_HANA_FWD_DROP_FRONT_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Drop the first `n` elements of an iterable, and return the rest.
    //! @relates Iterable
    //!
    //! Given an `Iterable` `xs` with a linearization of `[x1, x2, ...]` and
    //! a (non-negative) `Constant` `n` holding an unsigned integral value,
    //! `drop_front(xs, n)` is an iterable with the same tag whose
    //! linearization is `[xn+1, xn+2, ...]`. In particular, note that this
    //! function does not mutate the original iterable in any way. If `n` is
    //! not given, it defaults to a `Constant` with an unsigned integral value
    //! equal to `1`.
    //!
    //! In case `length(xs) <= n`, `drop_front` will simply drop the whole
    //! iterable without failing, thus returning an empty iterable. This is
    //! different from `drop_front_exactly`, which expects `n <= length(xs)`
    //! but can be better optimized because of this additional guarantee.
    //!
    //!
    //! @param xs
    //! The iterable from which elements are dropped.
    //!
    //! @param n
    //! A non-negative `Constant` holding an unsigned integral value
    //! representing the number of elements to be dropped from the iterable.
    //! If `n` is not given, it defaults to a `Constant` with an unsigned
    //! integral value equal to `1`.
    //!
    //!
    //! Example
    //! -------
    //! @include example/drop_front.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto drop_front = [](auto&& xs[, auto&& n]) {
        return tag-dispatched;
    };
#else
    template <typename It, typename = void>
    struct drop_front_impl : drop_front_impl<It, when<true>> { };

    struct drop_front_t {
        template <typename Xs, typename N>
        constexpr auto operator()(Xs&& xs, N&& n) const;

        template <typename Xs>
        constexpr auto operator()(Xs&& xs) const;
    };

    constexpr drop_front_t drop_front{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_DROP_FRONT_HPP
