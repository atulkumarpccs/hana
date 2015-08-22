/*!
@file
Forward declares `boost::hana::erase_key`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_ERASE_KEY_HPP
#define BOOST_HANA_FWD_ERASE_KEY_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! @todo Brief description
    //! @relates AssociativeSequence
    //!
    //! @todo Detailed description
    //!
    //!
    //! @param xs
    //! @todo
    //!
    //! @param ys
    //! @todo
    //!
    //!
    //! Example
    //! -------
    //! @include example/erase_key.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto erase_key = [](auto&& xs, auto&& key) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct erase_key_impl : erase_key_impl<S, when<true>> { };

    struct erase_key_t {
        template <typename Xs, typename Key>
        constexpr decltype(auto) operator()(Xs&& xs, Key&& key) const;
    };

    constexpr erase_key_t erase_key{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_ERASE_KEY_HPP
