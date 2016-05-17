/*!
@file
Defines `boost::hana::detail::canonicalize_foldable`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_CANONICALIZE_FOLDABLE_HPP
#define BOOST_HANA_DETAIL_CANONICALIZE_FOLDABLE_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/fwd/tuple.hpp>
#include <boost/hana/fwd/type.hpp>
#include <boost/hana/unpack.hpp>

#include <utility>


BOOST_HANA_NAMESPACE_BEGIN namespace detail {
    struct canonicalize_foldable_impl {
        template <typename ...T>
        auto operator()(T&& ...t) const -> hana::type<hana::tuple<T...>>;
    };

    //! @ingroup group-details
    //! Returns the type of a `hana::tuple` containing the contents of a
    //! `Foldable`.
    //!
    //! This is used to implement some pure-type algorithms more efficiently
    //! by avoiding a full-fledged `hana::unpack` to retrieve the contents of
    //! a `Foldable`.
    template <typename Xs>
    struct canonicalize_foldable {
        using type = typename decltype(
            hana::unpack(std::declval<Xs>(),
                         canonicalize_foldable_impl{})
        )::type;
    };

    template <typename ...T>
    struct canonicalize_foldable<hana::tuple<T...>> {
        using type = hana::tuple<T...>;
    };
} BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_DETAIL_CANONICALIZE_FOLDABLE_HPP
