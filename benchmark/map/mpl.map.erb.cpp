/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/mpl/at.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/inserter.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/transform.hpp>

#include <array>
#include "measure.hpp"
namespace mpl = boost::mpl;
namespace hana = boost::hana;


static constexpr int INPUT_SIZE = <%= input_size %>;
static constexpr int INDEX = (INPUT_SIZE * 2) / 3;
static constexpr int LOOPS = 10000;

template <typename T, typename Size>
struct make_array {
    using type = std::array<T, Size::value>;
};

void run() {
    using Range = mpl::range_c<int, 0, INPUT_SIZE>;
    using Arrays = mpl::transform<
        Range,
        mpl::pair<mpl::_, make_array<char, mpl::_>>,
        mpl::inserter<mpl::map<>, mpl::insert<mpl::_1, mpl::_2>>
    >::type;

    using Array = mpl::at<Arrays, mpl::integral_c<int, INDEX>>::type;
    constexpr Array array{};
    static_assert(array.size() == INDEX, "");
}

int main() {
    hana::benchmark::measure([] {
        volatile int c = LOOPS;
        for (int i = 0; i < c; ++i)
            run();
    });
}
