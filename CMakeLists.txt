cmake_minimum_required (VERSION 2.6)
project (isofind)

file(GLOB_RECURSE sources src/*.cpp)
add_executable(isofind ${sources})

find_library(MINISAT_LIBRARY NAMES minisat)
target_link_libraries(isofind ${MINISAT_LIBRARY})

target_include_directories(isofind PUBLIC src)
target_compile_options(isofind PUBLIC -O2 -std=c++11 -Wall -pedantic)
