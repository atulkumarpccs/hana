/*!
@file
Defines `boost::hana::fold_left`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOLD_LEFT_HPP
#define BOOST_HANA_FOLD_LEFT_HPP

#include <boost/hana/fwd/fold_left.hpp>

#include <boost/hana/concept/foldable.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/detail/variadic/foldl1.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/unpack.hpp>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename State, typename F>
    constexpr decltype(auto) fold_left_t::operator()(Xs&& xs, State&& state, F&& f) const {
        using S = typename hana::tag_of<Xs>::type;
        using FoldLeft = BOOST_HANA_DISPATCH_IF(fold_left_impl<S>,
            _models<Foldable, S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Foldable, S>::value,
        "hana::fold_left(xs, state, f) requires 'xs' to be Foldable");
    #endif

        return FoldLeft::apply(static_cast<Xs&&>(xs),
                               static_cast<State&&>(state),
                               static_cast<F&&>(f));
    }

    template <typename Xs, typename F>
    constexpr decltype(auto) fold_left_t::operator()(Xs&& xs, F&& f) const {
        using S = typename hana::tag_of<Xs>::type;
        using FoldLeft = BOOST_HANA_DISPATCH_IF(fold_left_impl<S>,
            _models<Foldable, S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Foldable, S>::value,
        "hana::fold_left(xs, f) requires 'xs' to be Foldable");
    #endif

        return FoldLeft::apply(static_cast<Xs&&>(xs), static_cast<F&&>(f));
    }
    //! @endcond

    template <typename T, bool condition>
    struct fold_left_impl<T, when<condition>> : default_ {
        // with state
        template <typename Xs, typename S, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, S&& s, F&& f) {
            return hana::unpack(static_cast<Xs&&>(xs),
                hana::partial(
                    detail::variadic::foldl1,
                    static_cast<F&&>(f),
                    static_cast<S&&>(s)
                )
            );
        }

        // without state
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return hana::unpack(static_cast<Xs&&>(xs),
                hana::partial(
                    detail::variadic::foldl1,
                    static_cast<F&&>(f)
                )
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLD_LEFT_HPP
