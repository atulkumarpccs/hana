/*!
@file
Defines `boost::hana::erase_key`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ERASE_KEY_HPP
#define BOOST_HANA_ERASE_KEY_HPP

#include <boost/hana/fwd/erase_key.hpp>

#include <boost/hana/core/dispatch.hpp>


namespace boost { namespace hana {
    struct AssociativeSequence; //! @todo include the forward declaration instead

    //! @cond
    template <typename Xs, typename Key>
    constexpr decltype(auto) erase_key_t::operator()(Xs&& xs, Key&& key) const {
        using S = typename datatype<Xs>::type;
        using EraseKey = BOOST_HANA_DISPATCH_IF(erase_key_impl<S>,
            AssociativeSequence<S>::value
        );

        return EraseKey::apply(static_cast<Xs&&>(xs), static_cast<Key&&>(key));
    }
    //! @endcond

    template <typename S, bool condition>
    struct erase_key_impl<S, when<condition>> : default_ {
        template <typename ...Args>
        static constexpr auto apply(Args&& ...) = delete;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ERASE_KEY_HPP
