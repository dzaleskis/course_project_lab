cmake_minimum_required(VERSION 3.19)
project(course_project_lab)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib)
include_directories(src)

add_executable(course_project_lab
        lib/CLI11.hpp
        lib/json.hpp
        lib/openGA.hpp
        src/element.hpp
        src/fitness.hpp
        src/iterator_utils.hpp
        src/main.cpp
        src/random_utils.hpp
        src/pass.hpp
        src/sorting_algorithm.hpp
        src/genetic_algorithm.hpp
        src/gaps.hpp src/classic_algorithms.hpp src/benchmarks.hpp src/benchmark_algorithms.hpp src/inversion_utils.hpp)
