cmake_minimum_required (VERSION 3.10)

add_subdirectory (randen)

if (TARGET randen)
    add_library (third_party::randen ALIAS randen)
endif ()
