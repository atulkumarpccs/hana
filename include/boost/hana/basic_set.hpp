/*!
@file
Defines `boost::hana::basic_set`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_BASIC_SET_HPP
#define BOOST_HANA_BASIC_SET_HPP

#include <boost/hana/fwd/basic_set.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/fast_and.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/erase_key.hpp>
#include <boost/hana/fold_left.hpp>
#include <boost/hana/fwd/any_of.hpp>
#include <boost/hana/fwd/contains.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/difference.hpp>
#include <boost/hana/fwd/find_if.hpp>
#include <boost/hana/fwd/intersection.hpp>
#include <boost/hana/fwd/is_subset.hpp>
#include <boost/hana/fwd/length.hpp>
#include <boost/hana/fwd/union.hpp>
#include <boost/hana/fwd/unpack.hpp>
#include <boost/hana/insert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/or.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    namespace detail {
        template <typename T, bool = std::is_empty<T>::value &&
                                     !std::is_final<T>::value>
        struct set_elt;

        template <typename T>
        struct set_elt<T, true> : T {
            constexpr set_elt() = default;

            template <typename U>
            explicit constexpr set_elt(U&& u)
                : T(static_cast<U&&>(u))
            { }

            constexpr T& get() & { return *this; }
            constexpr T const& get() const& { return *this; }
            constexpr T&& get() && { return static_cast<T&&>(*this); }
        };

        template <typename T>
        struct set_elt<T, false> {
            constexpr set_elt() = default;

            template <typename U>
            explicit constexpr set_elt(U&& u)
                : data_(static_cast<U&&>(u))
            { }

            T data_;

            constexpr T& get() & { return data_; }
            constexpr T const& get() const& { return data_; }
            constexpr T&& get() && { return static_cast<T&&>(data_); }
        };
    }

    template <typename ...T>
    struct basic_set : detail::set_elt<T>... {
        constexpr basic_set() = default;

        template <typename ...U>
        explicit constexpr basic_set(U&& ...u)
            : detail::set_elt<T>(static_cast<U&&>(u))...
        { }

        constexpr basic_set(basic_set const& other)
            : detail::set_elt<T>(static_cast<detail::set_elt<T> const&>(other).get())...
        { }

        constexpr basic_set(basic_set&& other)
            : detail::set_elt<T>(static_cast<detail::set_elt<T>&&>(other).get())...
        { }

        using hana_tag = basic_set_tag;
    };

    //////////////////////////////////////////////////////////////////////////
    // make<basic_set_tag>
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<basic_set_tag> {
        template <typename ...Xs>
        static constexpr basic_set<typename std::decay<Xs>::type...> apply(Xs&& ...xs) {
            return {static_cast<Xs&&>(xs)...};
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // equal
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<basic_set_tag, basic_set_tag> {
        template <typename S1, typename S2>
        static constexpr auto equal_helper(S1 const& s1, S2 const& s2, hana::true_)
        { return hana::is_subset(s1, s2); }

        template <typename S1, typename S2>
        static constexpr hana::false_ equal_helper(S1 const&, S2 const&, hana::false_)
        { return {}; }

        template <typename ...T, typename ...U>
        static constexpr auto apply(basic_set<T...> const& xs, basic_set<U...> const& ys) {
            return equal_impl::equal_helper(xs, ys, hana::bool_<sizeof...(T) == sizeof...(U)>{});
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // unpack
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<basic_set_tag> {
        template <typename ...T, typename F>
        static constexpr decltype(auto)
        apply(basic_set<T...> const& xs, F&& f) {
            return static_cast<F&&>(f)(
                static_cast<detail::set_elt<T> const&>(xs).get()...
            );
        }

        template <typename ...T, typename F>
        static constexpr decltype(auto)
        apply(basic_set<T...>& xs, F&& f) {
            return static_cast<F&&>(f)(
                static_cast<detail::set_elt<T>&>(xs).get()...
            );
        }

        template <typename ...T, typename F>
        static constexpr decltype(auto)
        apply(basic_set<T...>&& xs, F&& f) {
            return static_cast<F&&>(f)(
                static_cast<detail::set_elt<T>&&>(xs).get()...
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // length
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct length_impl<basic_set_tag> {
        template <typename ...T>
        static constexpr hana::size_t<sizeof...(T)> apply(basic_set<T...> const&)
        { return {}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // contains
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <bool ...b>
        struct fast_or {
            static constexpr bool value = !std::is_same<
                fast_or<b...>,
                fast_or<(b, false)...>
            >::value;
        };
    }

    template <>
    struct contains_impl<basic_set_tag> {
        template <typename ...T, typename U, typename = typename std::enable_if<
            hana::is_a<hana::type_tag, U>()
        >::type>
        static constexpr hana::bool_<
            std::is_base_of<hana::basic_type<typename U::type>, basic_set<T...>>::value
        > apply(basic_set<T...> const&, U const&)
        { return {}; }

        template <typename ...T, typename U, typename = typename std::enable_if<
            !hana::is_a<hana::type_tag, U>()
        >::type>
        static constexpr hana::bool_<
            detail::fast_or<
                decltype(hana::equal(std::declval<T>(), std::declval<U>()))::value...
            >::value
        > apply(basic_set<T...> const&, U const&)
        { return {}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // any_of
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct any_of_impl<basic_set_tag> {
        template <typename ...T, typename Pred>
        static constexpr auto apply(basic_set<T...> const& xs, Pred&& pred) {
            return hana::or_(pred(static_cast<detail::set_elt<T> const&>(xs).get())...);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // find_if
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <typename ...T> struct types;

        template <typename Pred, typename Types, typename = when<true>>
        struct find_type_of;

        template <typename Pred>
        struct find_type_of<Pred, types<>> { };

        template <typename Pred, typename T, typename ...Ts>
        struct find_type_of<Pred, types<T, Ts...>, when<
            hana::value<decltype(std::declval<Pred>()(std::declval<T>()))>()
        >> {
            using type = T;
        };

        template <typename Pred, typename T, typename ...Ts>
        struct find_type_of<Pred, types<T, Ts...>, when<
            !hana::value<decltype(std::declval<Pred>()(std::declval<T>()))>()
        >>
            : find_type_of<Pred, types<Ts...>>
        { };
    }

    template <>
    struct find_if_impl<basic_set_tag> {
        template <typename ...T, typename Pred, typename Found = typename detail::find_type_of<Pred, T...>::type>
        static constexpr auto apply(basic_set<T...>&& xs, Pred const&)
        { return hana::just(static_cast<detail::set_elt<Found>&&>(xs).get()); }

        template <typename ...T, typename Pred, typename Found = typename detail::find_type_of<Pred, T...>::type>
        static constexpr auto apply(basic_set<T...> const& xs, Pred const&)
        { return hana::just(static_cast<detail::set_elt<Found> const&>(xs).get()); }

        template <typename ...T, typename Pred, typename Found = typename detail::find_type_of<Pred, T...>::type>
        static constexpr auto apply(basic_set<T...>& xs, Pred const&)
        { return hana::just(static_cast<detail::set_elt<Found>&>(xs).get()); }


        template <typename ...T, typename Pred>
        static constexpr auto apply(basic_set<T...> const&, Pred const&, ...)
        { return hana::nothing; }
    };

    //////////////////////////////////////////////////////////////////////////
    // is_subset
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct is_subset_impl<basic_set_tag, basic_set_tag> {
        template <typename ...T, typename ...U>
        static constexpr auto apply(basic_set<T...> const&, basic_set<U...> const& ys)
            -> hana::bool_<detail::fast_and<
                hana::value<decltype(hana::contains(ys, std::declval<T>()))>()...
            >::value>
        { return {}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Conversions
    //////////////////////////////////////////////////////////////////////////
    template <typename F>
    struct to_impl<basic_set_tag, F, when<Foldable<F>::value>> {
        template <typename Xs>
        static constexpr auto apply(Xs&& xs) {
            return hana::fold_left(static_cast<Xs&&>(xs),
                                   hana::basic_set<>{},
                                   hana::insert);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // insert
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct insert_impl<basic_set_tag> {
        template <typename Xs, typename X>
        static constexpr auto insert_helper(Xs&& xs, X&&, hana::true_) {
            return static_cast<Xs&&>(xs);
        }

        template <typename ...T, typename U>
        static constexpr basic_set<T..., typename std::decay<U>::type>
        insert_helper(basic_set<T...> const& xs, U&& u, hana::false_) {
            return {
                static_cast<detail::set_elt<T> const&>(xs).get()...,
                static_cast<U&&>(u)
            };
        }

        template <typename ...T, typename U>
        static constexpr basic_set<T..., typename std::decay<U>::type>
        insert_helper(basic_set<T...>&& xs, U&& u, hana::false_) {
            return {
                static_cast<detail::set_elt<T>&&>(xs).get()...,
                static_cast<U&&>(u)
            };
        }

        template <typename Xs, typename X>
        static constexpr auto apply(Xs&& xs, X&& x) {
            return insert_impl::insert_helper(static_cast<Xs&&>(xs),
                                              static_cast<X&&>(x),
                                              decltype(hana::contains(xs, x)){});
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // erase_key
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct erase_key_impl<basic_set_tag> {
        template <typename Xs, typename X>
        static constexpr auto erase_key_helper(Xs&& xs, X&&, hana::false_) {
            return static_cast<Xs&&>(xs);
        }

        template <typename ...T, typename U>
        static constexpr auto
        erase_key_helper(basic_set<T...> const& xs, U&& u, hana::true_) {
            #warning TODO
        }

        template <typename Xs, typename X>
        static constexpr auto apply(Xs&& xs, X&& x) {
            return erase_key_impl::erase_key_helper(static_cast<Xs&&>(xs),
                                                    static_cast<X&&>(x),
                                                    decltype(hana::contains(xs, x)){});
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // intersection
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <typename Ys>
        struct basic_set_insert_if_contains {
            template <typename Result, typename Key>
            static constexpr auto helper(Result&& result, Key&& key, hana::true_) {
                return hana::insert(static_cast<Result&&>(result), static_cast<Key&&>(key));
            }

            template <typename Result, typename Key>
            static constexpr auto helper(Result&& result, Key&&, hana::false_) {
                return static_cast<Result&&>(result);
            }

            template <typename Result, typename Key>
            constexpr auto operator()(Result&& result, Key&& key) const {
                return basic_set_insert_if_contains::helper(
                        static_cast<Result&&>(result),
                        static_cast<Key&&>(key),
                        decltype(hana::contains(std::declval<Ys>(), key)){});
            }
        };
    }

    template <>
    struct intersection_impl<basic_set_tag> {
        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs&& xs, Ys const& ys) {
            return hana::fold_left(static_cast<Xs&&>(xs), hana::basic_set<>{},
                                   detail::basic_set_insert_if_contains<Ys>{});
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // union_
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct union_impl<basic_set_tag> {
        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs&& xs, Ys&& ys) {
            return hana::fold_left(static_cast<Xs&&>(xs), static_cast<Ys&&>(ys),
                                   hana::insert);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // difference
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct difference_impl<basic_set_tag> {
        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs&& xs, Ys&& ys) {
            return hana::fold_left(static_cast<Ys&&>(ys), static_cast<Xs&&>(xs),
                                   hana::erase_key);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_BASIC_SET_HPP
