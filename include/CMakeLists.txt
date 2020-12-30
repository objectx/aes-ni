
cmake_minimum_required (VERSION 3.10)

set (lib_ aes-ni-interface)
add_library (${lib_} INTERFACE)

target_include_directories (${lib_} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources (${lib_} PRIVATE
                clt/arg_type.hpp
                clt/aes-ni.hpp
                clt/benchmark.hpp
                clt/shuffle.hpp
                clt/rdrand.hpp
                clt/rng.hpp
                clt/statistics.hpp
                clt/util.hpp
                clt/detail/aen-ni_encdec_impl.hpp
                clt/detail/rng_inline.hpp
                clt/detail/aes-ni_inline.hpp
                clt/detail/aes-ni_key-exp_impl.hpp)
