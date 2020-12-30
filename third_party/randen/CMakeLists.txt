
cmake_minimum_required (VERSION 3.10)

set (lib_ randen)

add_library (${lib_} randen.h randen.cc vector128.h)
target_include_directories (${lib_} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if (TARGET aes-ni::settings)
    target_link_libraries (${lib_} PRIVATE aes-ni::settings)
endif ()
