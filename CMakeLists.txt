cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON")

# Determine this project is root-project or sub-project.
set(ROOT_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(ROOT_PROJECT ON)
  message(STATUS "CMake version: ${CMAKE_VERSION}")
  message(STATUS "This is the root project.")
endif()

project(aes-ni
        VERSION 0.0.1
        LANGUAGES CXX)

include (${CMAKE_BINARY_DIR}/conan_paths.cmake)

set_property (GLOBAL PROPERTY JOB_POOLS benchmark=1)

message("CMAKE_C_COMPILER = ${CMAKE_C_COMPILER}")
message("CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")

function (setvar_default_ var_)
    if (NOT DEFINED ${var_})
        set (${var_} ${ARGN} PARENT_SCOPE)
    endif ()
endfunction ()

setvar_default_ (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
setvar_default_ (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
setvar_default_ (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# get_property (is_multi_config_generator_ GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (ROOT_PROJECT AND (NOT CMAKE_BUILD_TYPE))
    set (CMAKE_BUILD_TYPE Debug)
endif ()

set (settings_ aes-ni-settings)
add_library (${settings_} INTERFACE)
target_compile_options (${settings_}
                        INTERFACE -Wall -Wextra -pedantic -march=native)
add_library (${settings_}-sanitizer INTERFACE)
target_compile_options (${settings_} INTERFACE -fsanitize=address,undefined)
target_link_options (${settings_} INTERFACE -fsanitize=address,undefined)

add_library (${settings_}-Debug INTERFACE)
target_compile_options (${settings_}-Debug INTERFACE -g -fno-omit-frame-pointer -UNDEBUG)
target_link_libraries (${settings_}-Debug INTERFACE ${settings_}-sanitizer)

add_library (${settings_}-Release INTERFACE)
target_compile_options (${settings_}-Release INTERFACE -Ofast)
target_compile_definitions (${settings_}-Release INTERFACE NDEBUG=1)

add_library (${settings_}-RelWithDebInfo INTERFACE)
target_compile_options (${settings_}-RelWithDebInfo INTERFACE -Ofast -g -UNDEBUG)
target_link_libraries (${settings_}-RelWithDebInfo INTERFACE ${settings_}-sanitizer)

add_library (${settings_}-MinSizeRel INTERFACE)
target_compile_options (${settings_}-MinSizeRel INTERFACE -Os -s)
target_compile_definitions (${settings_}-MinSizeRel INTERFACE NDEBUG=1)

target_link_libraries (${settings_} INTERFACE ${settings_}-$<CONFIG>)
add_library (aes-ni::settings ALIAS ${settings_})

# include("${CMAKE_CURRENT_SOURCE_DIR}/search_module.cmake")

find_package (llvm-openmp)
find_package (gmp)
find_package (fmt)
find_package (spdlog)

add_subdirectory (third_party)
add_subdirectory (include)

set (lib_ aes-ni)
add_library (${lib_}
             src/aes-ni.cpp
             src/benchmark.cpp
             src/rdrand.cpp
             src/rng.cpp
             src/shuffle.cpp
             src/util.cpp)
target_link_libraries (${lib_} PUBLIC aes-ni-interface
                               PRIVATE fmt::fmt spdlog::spdlog llvm-openmp::llvm-openmp gmp::gmp)
if (TARGET aes-ni::settings)
    target_link_libraries (${lib_} PRIVATE aes-ni::settings)
endif ()


if (ROOT_PROJECT)
    enable_testing()
    add_subdirectory(test)
    add_subdirectory(bench)
else()
  message(STATUS "Skip building benchmarks and tests.")
endif()
