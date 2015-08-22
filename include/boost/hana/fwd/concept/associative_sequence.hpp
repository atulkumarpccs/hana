/*!
@file
Forward declares `boost::hana::Applicative`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_CONCEPT_ASSOCIATIVE_SEQUENCE_HPP
#define BOOST_HANA_FWD_CONCEPT_ASSOCIATIVE_SEQUENCE_HPP

namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `AssociativeSequence` concept represents containers supporting
    //! set-theoretic operations.
    //!
    //! @todo Write a detailed description
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `intersection`, `union_` and `difference` satisfying the laws below.
    //!
    //!
    //! Laws
    //! ----
    //! @todo Figure out the laws
    //!
    //! This will also require `singleton`:
    //! @code
    //!     symmetric_difference(xs, ys) = (xs - ys) U (ys - xs)
    //!     erase_key(xs, x) = xs - {x}
    //!     insert(xs, x) = xs U {x}
    //!     insert_range(xs, foldable) = xs U to<Set>(foldable)
    //! @endcode
    //!
    //! Some laws that might make sense (more generally, we probably want to
    //! respect everything in a Set algebra):
    //! @code
    //!     union_(xs, xs) == xs
    //!     intersection(xs, xs) == xs
    //! @endcode
    //!
    //!
    //! Refined concept
    //! ---------------
    //! @todo Figure out whether we need something here.
    //!
    //!
    //! Concrete models
    //! ---------------
    //! `Set`, `Map`
    struct AssociativeSequence { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_CONCEPT_ASSOCIATIVE_SEQUENCE_HPP
