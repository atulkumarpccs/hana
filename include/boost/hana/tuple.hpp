/*!
@file
Defines `boost::hana::tuple`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TUPLE_HPP
#define BOOST_HANA_TUPLE_HPP

#include <boost/hana/fwd/tuple.hpp>

#include <boost/hana/basic_tuple.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/detail/decay.hpp>
#include <boost/hana/detail/fast_and.hpp>
#include <boost/hana/detail/index_if.hpp>
#include <boost/hana/detail/intrinsics.hpp>
#include <boost/hana/detail/operators/adl.hpp>
#include <boost/hana/detail/operators/comparable.hpp>
#include <boost/hana/detail/operators/iterable.hpp>
#include <boost/hana/detail/operators/monad.hpp>
#include <boost/hana/detail/operators/orderable.hpp>
#include <boost/hana/fwd/at.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/drop_front.hpp>
#include <boost/hana/fwd/find_if.hpp>
#include <boost/hana/fwd/is_empty.hpp>
#include <boost/hana/fwd/length.hpp>
#include <boost/hana/fwd/optional.hpp>
#include <boost/hana/fwd/unpack.hpp>
#include <boost/hana/type.hpp> // required by fwd decl of tuple_t

#include <cstddef>
#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    namespace detail {
        template <typename Xs, typename Ys, std::size_t ...n>
        constexpr void assign(Xs& xs, Ys&& ys, std::index_sequence<n...>) {
            int sequence[] = {int{}, ((void)(
                hana::get_impl<n>(xs) = hana::get_impl<n>(static_cast<Ys&&>(ys))
            ), int{})...};
            (void)sequence;
        }

        struct from_index_sequence_t { };
    }

    //////////////////////////////////////////////////////////////////////////
    // tuple
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <>
    struct tuple<>
        : detail::operators::adl<tuple<>>
        , detail::iterable_operators<tuple<>>
    {
        constexpr tuple() { }
        using hana_tag = tuple_tag;
    };

    template <typename ...Xn>
    struct tuple
        : detail::operators::adl<tuple<Xn...>>
        , detail::iterable_operators<tuple<Xn...>>
        , hana::basic_tuple<Xn...>
    {
        using Storage = hana::basic_tuple<Xn...>;
        using hana_tag = tuple_tag;

        Storage const& storage() const& { return *this; }
        Storage& storage() & { return *this; }
        Storage&& storage() && { return static_cast<Storage&&>(*this); }

    private:
        template <typename Other, std::size_t ...n>
        explicit constexpr tuple(detail::from_index_sequence_t, std::index_sequence<n...>, Other&& other)
            : Storage(hana::get_impl<n>(static_cast<Other&&>(other))...)
        { }

    public:
        template <typename ...dummy, typename = typename std::enable_if<
            detail::fast_and<BOOST_HANA_TT_IS_CONSTRUCTIBLE(Xn, dummy...)...>::value
        >::type>
        constexpr tuple()
            : Storage()
        { }

        template <typename ...dummy, typename = typename std::enable_if<
            detail::fast_and<BOOST_HANA_TT_IS_CONSTRUCTIBLE(Xn, Xn const&, dummy...)...>::value
        >::type>
        constexpr tuple(Xn const& ...xn)
            : Storage(xn...)
        { }

        template <typename ...Yn, typename = typename std::enable_if<
            detail::fast_and<BOOST_HANA_TT_IS_CONSTRUCTIBLE(Xn, Yn&&)...>::value
        >::type>
        constexpr tuple(Yn&& ...yn)
            : Storage(static_cast<Yn&&>(yn)...)
        { }

        template <typename ...Yn, typename = typename std::enable_if<
            detail::fast_and<BOOST_HANA_TT_IS_CONSTRUCTIBLE(Xn, Yn const&)...>::value
        >::type>
        constexpr tuple(tuple<Yn...> const& other)
            : tuple(detail::from_index_sequence_t{},
                    std::make_index_sequence<sizeof...(Xn)>{},
                    static_cast<typename tuple<Yn...>::Storage const&>(other))
        { }

        template <typename ...Yn, typename = typename std::enable_if<
            detail::fast_and<BOOST_HANA_TT_IS_CONSTRUCTIBLE(Xn, Yn&&)...>::value
        >::type>
        constexpr tuple(tuple<Yn...>&& other)
            : tuple(detail::from_index_sequence_t{},
                    std::make_index_sequence<sizeof...(Xn)>{},
                    static_cast<typename tuple<Yn...>::Storage&&>(other))
        { }

        // The three following constructors are required to make sure that
        // the tuple(Yn&&...) constructor is _not_ preferred over the copy
        // constructor for unary tuples containing a type that is constructible
        // from tuple<...>. See test/tuple/trap_construct.cpp
        template <typename ...dummy, typename = typename std::enable_if<
            detail::fast_and<BOOST_HANA_TT_IS_CONSTRUCTIBLE(Xn, Xn const&, dummy...)...>::value
        >::type>
        constexpr tuple(tuple const& other)
            : tuple(detail::from_index_sequence_t{},
                    std::make_index_sequence<sizeof...(Xn)>{},
                    static_cast<typename tuple::Storage const&>(other))
        { }

        template <typename ...dummy, typename = typename std::enable_if<
            detail::fast_and<BOOST_HANA_TT_IS_CONSTRUCTIBLE(Xn, Xn const&, dummy...)...>::value
        >::type>
        constexpr tuple(tuple& other)
            : tuple(const_cast<tuple const&>(other))
        { }

        template <typename ...dummy, typename = typename std::enable_if<
            detail::fast_and<BOOST_HANA_TT_IS_CONSTRUCTIBLE(Xn, Xn&&, dummy...)...>::value
        >::type>
        constexpr tuple(tuple&& other)
            : tuple(detail::from_index_sequence_t{},
                    std::make_index_sequence<sizeof...(Xn)>{},
                    static_cast<typename tuple::Storage&&>(other))
        { }


        template <typename ...Yn, typename = typename std::enable_if<
            detail::fast_and<BOOST_HANA_TT_IS_ASSIGNABLE(Xn&, Yn const&)...>::value
        >::type>
        constexpr tuple& operator=(tuple<Yn...> const& other) {
            detail::assign(static_cast<Storage&>(*this),
                           static_cast<typename tuple<Yn...>::Storage&>(other),
                           std::make_index_sequence<sizeof...(Xn)>{});
            return *this;
        }

        template <typename ...Yn, typename = typename std::enable_if<
            detail::fast_and<BOOST_HANA_TT_IS_ASSIGNABLE(Xn&, Yn&&)...>::value
        >::type>
        constexpr tuple& operator=(tuple<Yn...>&& other) {
            detail::assign(static_cast<Storage&>(*this),
                           static_cast<typename tuple<Yn...>::Storage&&>(other),
                           std::make_index_sequence<sizeof...(Xn)>{});
            return *this;
        }
    };
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <>
        struct comparable_operators<tuple_tag> {
            static constexpr bool value = true;
        };
        template <>
        struct orderable_operators<tuple_tag> {
            static constexpr bool value = true;
        };
        template <>
        struct monad_operators<tuple_tag> {
            static constexpr bool value = true;
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<tuple_tag> {
        template <typename F>
        static constexpr decltype(auto) apply(tuple<>&&, F&& f)
        { return static_cast<F&&>(f)(); }
        template <typename F>
        static constexpr decltype(auto) apply(tuple<>&, F&& f)
        { return static_cast<F&&>(f)(); }
        template <typename F>
        static constexpr decltype(auto) apply(tuple<> const&, F&& f)
        { return static_cast<F&&>(f)(); }

        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return hana::unpack(static_cast<Xs&&>(xs).storage(), static_cast<F&&>(f));
        }
    };

    template <>
    struct length_impl<tuple_tag> {
        template <typename ...Xs>
        static constexpr auto apply(tuple<Xs...> const&)
        { return hana::size_c<sizeof...(Xs)>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct at_impl<tuple_tag> {
        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N const&) {
            constexpr std::size_t index = N::value;
            return hana::get_impl<index>(static_cast<Xs&&>(xs).storage());
        }
    };

    template <>
    struct drop_front_impl<tuple_tag> {
        template <std::size_t N, typename Xs, std::size_t ...i>
        static constexpr auto helper(Xs&& xs, std::index_sequence<i...>) {
            return hana::make<tuple_tag>(hana::at_c<i+N>(static_cast<Xs&&>(xs))...);
        }

        template <typename Xs, typename N>
        static constexpr auto apply(Xs&& xs, N const&) {
            constexpr std::size_t len = decltype(hana::length(xs))::value;
            return helper<N::value>(static_cast<Xs&&>(xs), std::make_index_sequence<
                N::value < len ? len - N::value : 0
            >{});
        }
    };

    template <>
    struct is_empty_impl<tuple_tag> {
        template <typename ...Xs>
        static constexpr auto apply(tuple<Xs...> const&)
        { return hana::bool_c<sizeof...(Xs) == 0>; }
    };

    // compile-time optimizations (to reduce the # of function instantiations)
    template <std::size_t n, typename ...Xs>
    constexpr decltype(auto) at_c(tuple<Xs...> const& xs) {
        return hana::get_impl<n>(static_cast<typename tuple<Xs...>::Storage const&>(xs));
    }

    template <std::size_t n, typename ...Xs>
    constexpr decltype(auto) at_c(tuple<Xs...>& xs) {
        return hana::get_impl<n>(static_cast<typename tuple<Xs...>::Storage&>(xs));
    }

    template <std::size_t n, typename ...Xs>
    constexpr decltype(auto) at_c(tuple<Xs...>&& xs) {
        return hana::get_impl<n>(static_cast<typename tuple<Xs...>::Storage&&>(xs));
    }

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct Sequence<tuple_tag> {
        static constexpr bool value = true;
    };

    template <>
    struct make_impl<tuple_tag> {
        template <typename ...Xs>
        static constexpr
        tuple<typename detail::decay<Xs>::type...> apply(Xs&& ...xs)
        { return {static_cast<Xs&&>(xs)...}; }
    };

    template <>
    struct find_if_impl<tuple_tag> {
        template <std::size_t index, typename Xs>
        static constexpr auto helper(Xs&&, hana::true_) {
            return hana::nothing;
        }

        template <std::size_t index, typename Xs>
        static constexpr auto helper(Xs&& xs, hana::false_) {
            return hana::just(hana::at_c<index>(static_cast<Xs&&>(xs)));
        }

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&&) {
            using Pack = typename detail::make_pack<Xs>::type;
            constexpr std::size_t index = detail::index_if<Pred&&, Pack>::value;
            constexpr std::size_t len = Pack::length;
            return helper<index>(static_cast<Xs&&>(xs), hana::bool_c<index == len>);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TUPLE_HPP
