/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/at.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/unpack.hpp>

#include <array>
#include "measure.hpp"
namespace hana = boost::hana;


static constexpr int INPUT_SIZE = <%= input_size %>;
static constexpr int INDEX = (INPUT_SIZE * 2) / 3;
static constexpr int LOOPS = 10000;

void run() {
    auto arrays = hana::unpack(hana::range_c<int, 0, INPUT_SIZE>, [](auto ...n) {
        return hana::tuple_t<std::array<char, decltype(n)::value>...>;
    });

    auto Array = arrays[hana::int_c<INDEX>];
    constexpr decltype(Array)::type array{};
    static_assert(array.size() == INDEX, "");
}

int main() {
    hana::benchmark::measure([] {
        volatile int c = LOOPS;
        for (int i = 0; i < c; ++i)
            run();
    });
}
