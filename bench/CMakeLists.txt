
cmake_minimum_required (VERSION 3.10)

add_custom_target (run_benchmarks)

function (add_benchmark_ name_)
    set (bench_ bench_${name_})
    add_executable (${bench_} "${bench_}.cpp")
    target_link_libraries(${bench_} PRIVATE aes-ni spdlog::spdlog fmt::fmt gmp::gmp)
    if (TARGET aes-ni::settings)
        target_link_libraries (${bench_} PRIVATE aes-ni::settings)
    endif ()
    set (out_ "${CMAKE_CURRENT_BINARY_DIR}/bench_${name_}.csv")
    add_custom_target (run_${bench_}
                       COMMAND ${bench_} > ${out_}
                       COMMENT "# Benchmarking: ${name_}"
                       JOB_POOL benchmark)
    add_dependencies (run_benchmarks run_${bench_})
endfunction ()

# add_executable (benchmark-aes_ctr bench_aes_ctr.cpp)
# target_link_libraries (benchmark-aes_ctr aes-ni spdlog::spdlog fmt::fmt gmp::gmp aes-ni::settings)

add_benchmark_ (aes_ctr)
add_benchmark_ (aes_encdec)
add_benchmark_ (aesmmo)
add_benchmark_ (aesprf_ctr)
add_benchmark_ (aesprf)
add_benchmark_ (dev_urandom)
add_benchmark_ (randen)
add_benchmark_ (rdrand)
# suffle_fy
# suffle_rs

target_link_libraries (bench_randen PRIVATE third_party::randen)

# add_custom_target(run_benchmarks
#     COMMAND ln -s -f "${CMAKE_CURRENT_SOURCE_DIR}/run_benchmarks.sh"
#     COMMAND ln -s -f "${CMAKE_CURRENT_SOURCE_DIR}/benchmark.plot"
#     COMMAND ln -s -f "${CMAKE_CURRENT_SOURCE_DIR}/run_plotting.sh"
#     DEPENDS benchmarks
# )
