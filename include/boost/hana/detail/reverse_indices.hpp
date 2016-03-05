/*!
@file
Defines `boost::hana::detail::reverse_indices`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_REVERSE_INDICES_HPP
#define BOOST_HANA_DETAIL_REVERSE_INDICES_HPP

#include <boost/hana/config.hpp>

#include <cstddef>
#include <utility>


BOOST_HANA_NAMESPACE_BEGIN namespace detail {
     //! @ingroup group-details
    //! Reverses a `std::index_sequence`.
    //!
    //! Given a `std::index_sequence`, `reverse_indices` is a metafunction
    //! whose nested `::type` alias is a `std::index_sequence` whose elements
    //! are in reverse order.
    template <typename Indices>
    struct reverse_indices;

    template <std::size_t ...i>
    struct reverse_indices<std::index_sequence<i...>> {
        using type = std::index_sequence<(sizeof...(i) - i - 1)...>;
    };
} BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_DETAIL_REVERSE_INDICES_HPP
