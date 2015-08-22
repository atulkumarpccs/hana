/*!
@file
Defines `boost::hana::AssociativeSequence`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_ASSOCIATIVE_SEQUENCE_HPP
#define BOOST_HANA_CONCEPT_ASSOCIATIVE_SEQUENCE_HPP

#include <boost/hana/fwd/concept/associative_sequence.hpp>

#include <boost/hana/core/default.hpp>
#include <boost/hana/difference.hpp>
#include <boost/hana/fwd/core/models.hpp>
#include <boost/hana/intersection.hpp>
#include <boost/hana/union.hpp>


namespace boost { namespace hana {
    template <typename A>
    struct models_impl<AssociativeSequence, A> {
        static constexpr bool value =
            !hana::is_default<hana::intersection_impl<A>>::value &&
            !hana::is_default<hana::union_impl<A>>::value &&
            !hana::is_default<hana::difference_impl<A>>::value;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_ASSOCIATIVE_SEQUENCE_HPP
