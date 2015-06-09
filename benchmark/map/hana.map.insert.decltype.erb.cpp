/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/at_key.hpp>
#include <boost/hana/fold_left.hpp>
#include <boost/hana/insert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/type.hpp>

#include <array>
#include "measure.hpp"
namespace hana = boost::hana;


static constexpr int INPUT_SIZE = <%= input_size %>;
static constexpr int INDEX = (INPUT_SIZE * 2) / 3;
static constexpr int LOOPS = 10000;

void run() {
    auto get_array = [](auto index) {
        auto inserter = [](auto map, auto n) {
            using Array = std::array<char, decltype(n)::value>;
            return hana::insert(map, hana::make_pair(n, hana::type_c<Array>));
        };

        auto arrays = hana::fold_left(hana::range_c<int, 0, INPUT_SIZE>, hana::make_map(), inserter);
        return arrays[index];
    };

    constexpr decltype(get_array(hana::int_c<INDEX>))::type array{};
    static_assert(array.size() == INDEX, "");
}

int main() {
    hana::benchmark::measure([] {
        volatile int c = LOOPS;
        for (int i = 0; i < c; ++i)
            run();
    });
}
