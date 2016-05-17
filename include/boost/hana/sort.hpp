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
#include <boost/hana/fwd/tuple.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/less.hpp>

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

    // Implementation based on Brigand (https://github.com/edouarda/brigand).
    namespace sort_detail {
        template <std::size_t index, typename T>
        struct pair { using type = T; };

        template <typename ...T>
        struct list { };

        template <template <typename ...> class Pred>
        struct S {
            // Out: a sorted list of types to insert into (used as an accumulator, must be empty initially)
            // In: a sorted list of type to insert into Out
            // Tag: Whether the first element of Ts... is less than the first element of In
            // FTag: Whether the 8th element of Ts... is less than the first element of In
            // Ts...: A sorted list of types to insert into Out
            template <typename Out, typename In, bool Tag, bool FTag, typename ...Ts>
            struct merge_impl;

            // next is not less than insert, no more ins, terminate
            template <typename ...Os, typename In, typename ...Ts>
            struct merge_impl<list<Os...>, list<In>, false, false, Ts...> {
                using type = list<Os..., In, Ts...>;
            };

            // next is less than insert, next is end, terminate
            template <typename ...Os, typename ...Ins, typename T>
            struct merge_impl<list<Os...>, list<Ins...>, true, false, T> {
                using type = list<Os..., T, Ins...>;
            };

            // next is not less than insert, have more next and have more ins, cant fast track
            template <typename ...Os, typename In1, typename In2, typename ...Ins, typename T, typename ...Ts>
            struct merge_impl<list<Os...>, list<In1, In2, Ins...>, false, false, T, Ts...>
                : merge_impl<
                    list<Os..., In1>,
                    list<In2, Ins...>,
                    Pred<T, In2>::value,
                    false,
                    T, Ts...
                >
            { };

            // next is not less than insert, have more next and have more ins, can fast track
            template <typename ...Os, typename In1, typename In2, typename ...Ins, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename ...Ts>
            struct merge_impl<list<Os...>, list<In1, In2, Ins...>, false, false, T1, T2, T3, T4, T5, T6, T7, T8, Ts...>
                : merge_impl<
                    list<Os..., In1>,
                    list<In2, Ins...>,
                    Pred<T1, In2>::value,
                    Pred<T8, In2>::value,
                    T1, T2, T3, T4, T5, T6, T7, T8, Ts...
                >
            { };

            // next is less than insert, next is not end
            template <typename ...Os, typename In, typename ...Ins, typename T1, typename T2, typename ...Ts>
            struct merge_impl<list<Os...>, list<In, Ins...>, true, false, T1, T2, Ts...>
                : merge_impl<
                    list<Os..., T1>,
                    list<In, Ins...>,
                    Pred<T2, In>::value,
                    false,
                    T2, Ts...
                >
            { };

            // fast track is less than insert, no more
            template <typename ...Os, typename In, typename ...Ins, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
            struct merge_impl<list<Os...>, list<In, Ins...>, true, true, T1, T2, T3, T4, T5, T6, T7, T8> {
                using type = list<Os..., T1, T2, T3, T4, T5, T6, T7, T8, In, Ins...>;
            };

            // fast track is less than insert, not enough to fast track again
            template <typename ...Os, typename In, typename ...Ins, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename ...Ts>
            struct merge_impl<list<Os...>, list<In, Ins...>, true, true, T1, T2, T3, T4, T5, T6, T7, T8, T9, Ts...>
                : merge_impl<
                    list<Os..., T1, T2, T3, T4, T5, T6, T7, T8>,
                    list<In, Ins...>,
                    Pred<T9, In>::value,
                    false,
                    T9, Ts...
                >
            { };

            // fast track is less than insert, can fast track again
            template <typename ...Os, typename In, typename ...Ins, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename ...Ts>
            struct merge_impl<list<Os...>, list<In, Ins...>, true, true, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, Ts...>
                : merge_impl<
                    list<Os..., T1, T2, T3, T4, T5, T6, T7, T8>,
                    list<In, Ins...>,
                    Pred<T9, In>::value,
                    Pred<T16, In>::value,
                    T9, T10, T11, T12, T13, T14, T15, T16, Ts...
                >
            { };

            // Out: a sorted list in which to insert
            // In: a sorted list whose elements are inserted at the right place in Out
            template <typename Out, typename In>
            struct merge;

            // Too few types to insert; can't batch them.
            template <typename O, typename ...Os, typename In, typename ...Ins>
            struct merge<list<O, Os...>, list<In, Ins...>>
                : merge_impl<list<>, list<In, Ins...>, Pred<O, In>::value, false, O, Os...>
            { };

            // Enough types to insert; check if we can insert all of Ins after Os.
            template <typename O1, typename O2, typename O3, typename O4, typename O5, typename O6, typename O7, typename O8, typename ...Os, typename In, typename ...Ins>
            struct merge<list<O1, O2, O3, O4, O5, O6, O7, O8, Os...>, list<In, Ins...>>
                : merge_impl<
                    list<>,
                    list<In, Ins...>,
                    Pred<O1, In>::value,
                    Pred<O8, In>::value,
                    O1, O2, O3, O4, O5, O6, O7, O8, Os...
                >
            { };

            // Out: a sorted list of types
            // Ts...: unsorted types to insert at the right spot in Out
            template <typename Out, typename ...Ts>
            struct sort_loop;

            // No more types to insert.
            template <typename Out>
            struct sort_loop<Out> {
                using type = Out;
            };

            // Too few types to insert; can't batch them.
            template <typename Out, typename T1, typename ...Ts>
            struct sort_loop<Out, T1, Ts...> {
                using type =  typename sort_loop<
                    typename merge<Out, list<T1>>::type,
                    Ts...
                >::type;
            };

            // Enough types to insert; sort a batch of 16 and deal with the rest recursively.
            template <typename Out, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename ...Ts>
            struct sort_loop<Out, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, Ts...> {
                using Batch  = typename sort_loop<
                    list<T1>,
                    T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16
                >::type;
                using Rest = typename sort_loop<Out, Ts...>::type;
                using type = typename merge<Rest, Batch>::type;
            };
        };

        template <typename Pred>
        struct sort_comp {
            template <typename A, typename B>
            using apply = std::integral_constant<bool, (bool)decltype(
                std::declval<Pred>()(
                    std::declval<typename A::type>(),
                    std::declval<typename B::type>()
                )
            )::value>;
        };


        template <typename Pred, typename Indices, typename Tuple>
        struct sort_indices;

        template <typename Pred, std::size_t i0, std::size_t ...in, typename T0, typename ...Tn>
        struct sort_indices<Pred, std::index_sequence<i0, in...>, hana::tuple<T0, Tn...>> {
            using type = typename S<sort_comp<Pred>::template apply>::template sort_loop<
                list<pair<i0, T0>>,
                pair<in, Tn>...
            >::type;
        };

        template <typename Pred>
        struct sort_indices<Pred, std::index_sequence<>, hana::tuple<>> {
            using type = list<>;
        };

        template <typename Pred, typename T>
        struct sort_indices<Pred, std::index_sequence<0>, hana::tuple<T>> {
            using type = list<pair<0, T>>;
        };
    } // end namespace sort_detail

    template <typename S, bool condition>
    struct sort_impl<S, when<condition>> : default_ {
        template <typename Xs, std::size_t ...i, typename ...T>
        static constexpr auto apply_impl(Xs&& xs, sort_detail::list<sort_detail::pair<i, T>...>) {
            return hana::make<S>(hana::at_c<i>(static_cast<Xs&&>(xs))...);
        }

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred const&) {
            constexpr std::size_t Len = decltype(hana::length(xs))::value;
            using Raw = typename std::remove_cv<
                    typename std::remove_reference<Xs>::type
                >::type;
            using Canonical = typename detail::canonicalize_foldable<Raw>::type;
            using Indices = typename sort_detail::sort_indices<
                Pred const&,
                std::make_index_sequence<Len>,
                Canonical
            >::type;
            return apply_impl(static_cast<Xs&&>(xs), Indices{});
        }

        template <typename Xs>
        static constexpr auto apply(Xs&& xs)
        { return sort_impl::apply(static_cast<Xs&&>(xs), hana::less); }
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_SORT_HPP
