/*!
@file
Defines `boost::hana::sort`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SORT_HPP
#define BOOST_HANA_SORT_HPP

#include <boost/hana/fwd/sort.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/detail/canonicalize_foldable.hpp>
#include <boost/hana/detail/nested_by.hpp> // required by fwd decl
#include <boost/hana/length.hpp>
#include <boost/hana/less.hpp>

#include <type_traits>
#include <utility> // std::declval, std::index_sequence


BOOST_HANA_NAMESPACE_BEGIN
    //! @cond
    template <typename Xs, typename Predicate>
    constexpr auto sort_t::operator()(Xs&& xs, Predicate&& pred) const {
        using S = typename hana::tag_of<Xs>::type;
        using Sort = BOOST_HANA_DISPATCH_IF(sort_impl<S>,
            hana::Sequence<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(hana::Sequence<S>::value,
        "hana::sort(xs, predicate) requires 'xs' to be a Sequence");
    #endif

        return Sort::apply(static_cast<Xs&&>(xs),
                           static_cast<Predicate&&>(pred));
    }

    template <typename Xs>
    constexpr auto sort_t::operator()(Xs&& xs) const {
        using S = typename hana::tag_of<Xs>::type;
        using Sort = BOOST_HANA_DISPATCH_IF(sort_impl<S>,
            hana::Sequence<S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(hana::Sequence<S>::value,
        "hana::sort(xs) requires 'xs' to be a Sequence");
    #endif

        return Sort::apply(static_cast<Xs&&>(xs));
    }
    //! @endcond

    namespace detail {
        template <typename Pred>
        struct sort_predicate {
            template <typename T, typename U>
            using apply = decltype(std::declval<Pred>()(
                std::declval<typename T::type>(),
                std::declval<typename U::type>()
            ));
        };

        template <typename ...>
        struct list { };

        template <std::size_t i, typename T>
        struct pair { using type = T; };

        template <typename Pred, typename Insert, bool IsInsertionPoint,
                  typename Left,
                  typename ...Right>
        struct insert;

        // We did not find the insertion point; continue processing elements
        // recursively.
        template <
            typename Pred, typename Insert,
            typename ...Left,
            typename Right1, typename Right2, typename ...Right
        >
        struct insert<Pred, Insert, false,
                      list<Left...>,
                      Right1, Right2, Right...
        > {
            using type = typename insert<
                Pred, Insert, (bool)Pred::template apply<Insert, Right2>::value,
                list<Left..., Right1>,
                Right2, Right...
            >::type;
        };

        // We did not find the insertion point, but there is only one element
        // left. We insert at the end of the list, and we're done.
        template <typename Pred, typename Insert, typename ...Left, typename Last>
        struct insert<Pred, Insert, false, list<Left...>, Last> {
            using type = list<Left..., Last, Insert>;
        };

        // We found the insertion point, we're done.
        template <typename Pred, typename Insert, typename ...Left, typename ...Right>
        struct insert<Pred, Insert, true, list<Left...>, Right...> {
            using type = list<Left..., Insert, Right...>;
        };


        template <typename Pred, typename Result, typename ...T>
        struct insertion_sort_impl;

        template <typename Pred,
                  typename Result1, typename ...Result,
                  typename T, typename ...Ts>
        struct insertion_sort_impl<Pred, list<Result1, Result...>, T, Ts...> {
            using type = typename insertion_sort_impl<
                Pred,
                typename insert<
                    Pred, T, (bool)Pred::template apply<T, Result1>::value,
                    list<>,
                    Result1, Result...
                >::type,
                Ts...
            >::type;
        };

        template <typename Pred, typename T, typename ...Ts>
        struct insertion_sort_impl<Pred, list<>, T, Ts...> {
            using type = typename insertion_sort_impl<
                Pred, list<T>, Ts...
            >::type;
        };

        template <typename Pred, typename Result>
        struct insertion_sort_impl<Pred, Result> {
            using type = Result;
        };

        template <typename Pred, typename Tuple, typename Indices>
        struct sort_helper;

        template <typename Pred, typename ...T, std::size_t ...i>
        struct sort_helper<Pred, hana::tuple<T...>, std::index_sequence<i...>> {
            using type = typename insertion_sort_impl<
                Pred, list<>, pair<i, T>...
            >::type;
        };
    } // end namespace detail

    template <typename S, bool condition>
    struct sort_impl<S, when<condition>> : default_ {
        template <typename Xs, std::size_t ...i, typename ...T>
        static constexpr auto apply_impl(Xs&& xs, detail::list<detail::pair<i, T>...>) {
            return hana::make<S>(hana::at_c<i>(static_cast<Xs&&>(xs))...);
        }

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred const&) {
            constexpr std::size_t Len = decltype(hana::length(xs))::value;
            using Raw = typename std::remove_cv<
                typename std::remove_reference<Xs>::type
            >::type;
            using Indices = typename detail::sort_helper<
                detail::sort_predicate<Pred>,
                typename detail::canonicalize_foldable<Raw>::type,
                std::make_index_sequence<Len>
            >::type;

            return apply_impl(static_cast<Xs&&>(xs), Indices{});
        }

        template <typename Xs>
        static constexpr auto apply(Xs&& xs)
        { return sort_impl::apply(static_cast<Xs&&>(xs), hana::less); }
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_SORT_HPP
