/*!
@file
Forward declares `boost::hana::is_disjoint`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_IS_DISJOINT_HPP
#define BOOST_HANA_FWD_IS_DISJOINT_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! @ingroup group-algorithms-set
    //! Returns whether two `Searchable`s are disjoint.
    //!
    //! Given two `Searchable`s `xs` and `ys`, `is_disjoint` returns a
    //! `Logical` representing whether the keys in `xs` are disjoint from
    //! the keys in `ys`, i.e. whether both structures have no keys in common.
    //!
    //!
    //! @param xs, ys
    //! Two `Searchable`s to test for disjointness.
    //!
    //!
    //! Example
    //! -------
    //! @include example/is_disjoint.cpp
    //!
    //!
    //! Benchmarks
    //! ----------
    //! <div class="benchmark-chart"
    //!      style="min-width: 310px; height: 400px; margin: 0 auto"
    //!      data-dataset="benchmark.is_disjoint.compile.json">
    //! </div>
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto is_disjoint = [](auto const& xs, auto const& ys) {
        return tag-dispatched;
    };
#else
    template <typename S1, typename S2, typename = void>
    struct is_disjoint_impl : is_disjoint_impl<S1, S2, when<true>> { };

    struct is_disjoint_t {
        template <typename Xs, typename Ys>
        constexpr auto operator()(Xs&& xs, Ys&& ys) const;
    };

    constexpr is_disjoint_t is_disjoint{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_IS_DISJOINT_HPP
