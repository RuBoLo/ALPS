cmake_minimum_required(VERSION 3.16)

project(ALPS VERSION 0.0.0 LANGUAGES CXX)

add_library(alps STATIC)

target_include_directories(alps
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_compile_features(alps PRIVATE
    cxx_std_20
)

target_compile_options(alps PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -O3
)

find_library(CFITSIO_LIBRARY cfitsio REQUIRED)

target_link_libraries(alps PRIVATE
    ${CFITSIO_LIBRARY}
)

add_subdirectory(src)
add_subdirectory(cli)
