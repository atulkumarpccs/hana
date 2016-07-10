# Copyright Louis Dionne 2013-2016
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

set(datasets)
add_dataset(datasets dataset.fold_left.fusion.list      fusion.list.cpp.erb      "(0...50).step(5)")
add_dataset(datasets dataset.fold_left.fusion.vector    fusion.vector.cpp.erb    "(0...50).step(5)")
add_dataset(datasets dataset.fold_left.hana.basic_tuple hana.basic_tuple.cpp.erb "(0...50).step(5).to_a + (50..400).step(25).to_a")
add_dataset(datasets dataset.fold_left.hana.tuple       hana.tuple.cpp.erb       "(0...50).step(5).to_a + (50..400).step(25).to_a")
add_dataset(datasets dataset.fold_left.mpl.vector       mpl.vector.cpp.erb       "(0...50).step(5)")

add_chart(benchmark.fold_left DATASETS ${datasets}
    TITLE "Compile-time behavior of fold_left"
    SUBTITLE "(smaller is better)"
)
