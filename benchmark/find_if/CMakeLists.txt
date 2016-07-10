# Copyright Louis Dionne 2013-2016
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

set(datasets)
add_dataset(datasets dataset.find_if.fusion.list          fusion.list.cpp.erb          "(0...50).step(5)")
add_dataset(datasets dataset.find_if.fusion.vector        fusion.vector.cpp.erb        "(0...50).step(5)")
add_dataset(datasets dataset.find_if.hana.map             hana.map.cpp.erb             "(0...50).step(5).to_a + (50..200).step(25).to_a")
add_dataset(datasets dataset.find_if.hana.set             hana.set.cpp.erb             "(0...50).step(5).to_a + (50..200).step(25).to_a")
add_dataset(datasets dataset.find_if.hana.tuple           hana.tuple.cpp.erb           "(0...50).step(5).to_a + (50..200).step(25).to_a")
add_dataset(datasets dataset.find_if.mpl.vector           mpl.vector.cpp.erb           "(0...50).step(5)")
add_dataset(datasets dataset.find_if.std.integer_sequence std.integer_sequence.cpp.erb "(0...50).step(5).to_a + (50..200).step(25).to_a")

add_chart(benchmark.find_if DATASETS ${datasets}
    TITLE "Compile-time behavior of find_if"
    SUBTITLE "(smaller is better)"
)
