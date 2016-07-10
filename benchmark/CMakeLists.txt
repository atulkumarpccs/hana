# Copyright Louis Dionne 2013-2016
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Required packages, gems and caveats
##############################################################################
find_package(Ruby 2.1)
if(NOT ${RUBY_FOUND})
    message(WARNING "Ruby >= 2.1 was not found; the benchmarks will be unavailable.")
    return()
endif()

# Check for the 'ruby-progressbar' and 'tilt' gems
execute_process(COMMAND ${RUBY_EXECUTABLE} -r ruby-progressbar -r tilt -e ""
                RESULT_VARIABLE __BOOST_HANA_MISSING_GEMS
                OUTPUT_QUIET ERROR_QUIET)
if(${__BOOST_HANA_MISSING_GEMS})
    message(WARNING
        "The 'ruby-progressbar' and/or 'tilt' gems were not found; "
        "the benchmarks will be unavailable."
        "Use `gem install ruby-progressbar tilt` to install the missing gems.")
    return()
endif()

# Check for the MPL11 library, which can be measured in some benchmarks
find_package(MPL11)
if (MPL11_FOUND)
    include_directories(${MPL11_INCLUDE_DIR})
endif()

# Check for the Meta library, which can be measured in some benchmarks
find_package(Meta)
if (Meta_FOUND)
    include_directories(${Meta_INCLUDE_DIR})
endif()

boost_hana_append_flag(BOOST_HANA_HAS_FTEMPLATE_DEPTH -ftemplate-depth=-1)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

##############################################################################
# Configure the measure.rb script
##############################################################################
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/measure.in.rb #input
               ${CMAKE_CURRENT_BINARY_DIR}/measure.rb    #output
               @ONLY)

##############################################################################
# Add the benchmarks
##############################################################################
add_custom_target(benchmarks COMMENT "Generate all the benchmarks.")

add_custom_target(travis_compiler_slug USES_TERMINAL
    COMMAND ${CMAKE_COMMAND} -E echo "travis_compiler_slug: $<LOWER_CASE:${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}>")
add_custom_target(travis_config_slug USES_TERMINAL
    COMMAND ${CMAKE_COMMAND} -E echo "travis_config_slug: $<LOWER_CASE:$<CONFIG>>")

file(GLOB_RECURSE BOOST_HANA_BENCHMARKS *.erb.json)
foreach(benchmark IN LISTS BOOST_HANA_BENCHMARKS)
    boost_hana_target_name_for(target ${benchmark} ".erb.json")
    get_filename_component(directory "${benchmark}" DIRECTORY)
    file(GLOB cpp_files ${directory}/*.erb.cpp)
    configure_file("${benchmark}" "${CMAKE_CURRENT_BINARY_DIR}/${target}.erb.json" @ONLY)

    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${target}.measure.cpp "")
    add_executable(${target}.measure EXCLUDE_FROM_ALL ${CMAKE_CURRENT_BINARY_DIR}/${target}.measure.cpp)
    set_target_properties(${target}.measure PROPERTIES RULE_LAUNCH_COMPILE "${CMAKE_CURRENT_BINARY_DIR}/measure.rb")
    set_property(TARGET ${target}.measure APPEND PROPERTY INCLUDE_DIRECTORIES "${directory}")
    add_custom_target(${target}.measure.run COMMAND ${target}.measure)

    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${target}.json"
        COMMAND ${RUBY_EXECUTABLE} -r tilt -r ${CMAKE_CURRENT_BINARY_DIR}/measure.rb
            -e "MEASURE_FILE = '${CMAKE_CURRENT_BINARY_DIR}/${target}.measure.cpp'"
            -e "MEASURE_TARGET = '${target}.measure'"
            -e "json = Tilt::ERBTemplate.new('${CMAKE_CURRENT_BINARY_DIR}/${target}.erb.json').render"
            -e "File.open('${CMAKE_CURRENT_BINARY_DIR}/${target}.json', 'w') { |f| f.write(json) } "
        WORKING_DIRECTORY ${directory}
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${target}.erb.json ${cpp_files}
        VERBATIM USES_TERMINAL
        COMMENT "Generating dataset for ${target}"
    )

    add_custom_target(${target} DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${target}.json")
    add_dependencies(benchmarks ${target})
endforeach()


##############################################################################
# New Metabench-based benchmark suite
##############################################################################

include(metabench)

macro(add_dataset datasets dataset path_to_template range)
    string(REGEX REPLACE "[^.]+[.][^.]+[.](.*)" "\\1" name ${dataset})
    set(args ${path_to_template} ${range} NAME ${name} ${ARGN})

    if(${dataset} MATCHES "fusion\\.|mpl\\.")
        if (Boost_FOUND)
            metabench_add_dataset(${dataset} ${args} MEDIAN_OF 3)
            target_include_directories(${dataset} PUBLIC ${Boost_INCLUDE_DIRS})
        endif()
    elseif(${dataset} MATCHES "hana\\.")
        metabench_add_dataset(${dataset} ${args} MEDIAN_OF 3)
        target_include_directories(${dataset} PUBLIC ${Hana_INCLUDE_DIRS})
    elseif (${dataset} MATCHES "std\\.")
        metabench_add_dataset(${dataset} ${args} MEDIAN_OF 3)
    else()
        message(FATAL_ERROR
            "The dataset named '${dataset}' does not appear to benchmark any "
            "known metaprogramming library."
        )
    endif()

    if (TARGET ${dataset})
        list(APPEND ${datasets} ${dataset})
    endif()
endmacro()

macro(add_chart target)
    metabench_add_chart(${target} ${ARGN})
    add_dependencies(benchmarks ${target})
endmacro()

foreach(benchmark_dir find_if fold_left)
    add_subdirectory(${benchmark_dir})
endforeach()
