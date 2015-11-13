/*!
@file
Defines `boost::hana::pair`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PAIR_HPP
#define BOOST_HANA_PAIR_HPP

#include <boost/hana/fwd/pair.hpp>

#include <boost/hana/basic_tuple.hpp>
#include <boost/hana/detail/decay.hpp>
#include <boost/hana/detail/intrinsics.hpp>
#include <boost/hana/detail/operators/adl.hpp>
#include <boost/hana/detail/operators/comparable.hpp>
#include <boost/hana/detail/operators/orderable.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/first.hpp>
#include <boost/hana/fwd/second.hpp>

#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // pair
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename First, typename Second>
    struct pair
        : detail::operators::adl<pair<First, Second>>
        , hana::basic_tuple<First, Second>
    {
        using Storage = hana::basic_tuple<First, Second>;
        using hana_tag = pair_tag;

        template <typename ...dummy, typename = typename std::enable_if<
            BOOST_HANA_TT_IS_CONSTRUCTIBLE(First, dummy...) &&
            BOOST_HANA_TT_IS_CONSTRUCTIBLE(Second, dummy...)
        >::type>
        constexpr pair()
            : Storage()
        { }

        template <typename ...dummy, typename = typename std::enable_if<
            BOOST_HANA_TT_IS_CONSTRUCTIBLE(First, First const&, dummy...) &&
            BOOST_HANA_TT_IS_CONSTRUCTIBLE(Second, Second const&, dummy...)
        >::type>
        constexpr pair(First const& first, Second const& second)
            : Storage{first, second}
        { }

        template <typename T, typename U, typename = typename std::enable_if<
            BOOST_HANA_TT_IS_CONVERTIBLE(T&&, First) &&
            BOOST_HANA_TT_IS_CONVERTIBLE(U&&, Second)
        >::type>
        constexpr pair(T&& t, U&& u)
            : Storage{static_cast<T&&>(t), static_cast<U&&>(u)}
        { }

        template <typename T, typename U, typename = typename std::enable_if<
            BOOST_HANA_TT_IS_CONVERTIBLE(T const&, First) &&
            BOOST_HANA_TT_IS_CONVERTIBLE(U const&, Second)
        >::type>
        constexpr pair(pair<T, U> const& other)
            : Storage{hana::get_impl<0>(other),
                      hana::get_impl<1>(other)}
        { }

        template <typename T, typename U, typename = typename std::enable_if<
            BOOST_HANA_TT_IS_CONVERTIBLE(T&&, First) &&
            BOOST_HANA_TT_IS_CONVERTIBLE(U&&, Second)
        >::type>
        constexpr pair(pair<T, U>&& other)
            : Storage{static_cast<T&&>(hana::get_impl<0>(other)),
                      static_cast<U&&>(hana::get_impl<1>(other))}
        { }

        template <typename T, typename U, typename = typename std::enable_if<
            BOOST_HANA_TT_IS_ASSIGNABLE(First&, T const&) &&
            BOOST_HANA_TT_IS_ASSIGNABLE(Second&, U const&)
        >::type>
        constexpr pair& operator=(pair<T, U> const& other) {
            hana::get_impl<0>(*this) = hana::get_impl<0>(other);
            hana::get_impl<1>(*this) = hana::get_impl<1>(other);
            return *this;
        }

        template <typename T, typename U, typename = typename std::enable_if<
            BOOST_HANA_TT_IS_ASSIGNABLE(First&, T&&) &&
            BOOST_HANA_TT_IS_ASSIGNABLE(Second&, U&&)
        >::type>
        constexpr pair& operator=(pair<T, U>&& other) {
            hana::get_impl<0>(*this) = static_cast<T&&>(hana::get_impl<0>(other));
            hana::get_impl<1>(*this) = static_cast<U&&>(hana::get_impl<1>(other));
            return *this;
        }
    };
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <>
        struct comparable_operators<pair_tag> {
            static constexpr bool value = true;
        };
        template <>
        struct orderable_operators<pair_tag> {
            static constexpr bool value = true;
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // Product
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<pair_tag> {
        template <typename F, typename S>
        static constexpr pair<
            typename detail::decay<F>::type,
            typename detail::decay<S>::type
        > apply(F&& f, S&& s) {
            return {static_cast<F&&>(f), static_cast<S&&>(s)};
        }
    };

    template <>
    struct first_impl<pair_tag> {
        template <typename P>
        static constexpr decltype(auto) apply(P&& p)
        { return hana::get_impl<0>(static_cast<P&&>(p)); }
    };

    template <>
    struct second_impl<pair_tag> {
        template <typename P>
        static constexpr decltype(auto) apply(P&& p)
        { return hana::get_impl<1>(static_cast<P&&>(p)); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PAIR_HPP
