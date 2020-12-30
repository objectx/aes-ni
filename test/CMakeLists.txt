find_package(GTest REQUIRED)

add_library (test-aes-ni-common INTERFACE)
target_link_libraries (test-aes-ni-common INTERFACE aes-ni spdlog::spdlog fmt::fmt GTest::GTest gmp::gmp)

function(add_unit_test testname)
    add_executable(${testname} ${testname}.cpp)
    target_link_libraries(${testname} PRIVATE test-aes-ni-common)
    if (TARGET aes-ni::settings)
        target_link_libraries (${testname} PRIVATE aes-ni::settings)
    endif ()
    add_test(NAME    ${testname}
             COMMAND ${testname})
endfunction()

add_unit_test(test_aes-ni)
add_unit_test(test_util)
add_unit_test(test_shuffle)
