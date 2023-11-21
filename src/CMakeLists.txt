cmake_minimum_required(VERSION 3.5)

project(deadparrot
        VERSION 0.0
        LANGUAGES C
        DESCRIPTION "use old removed Python C functions on new Python versions")

set(Python_VERSION "" CACHE STRING "Optional user-selected Python version")
if(Python_VERSION)
find_package (Python ${Python_VERSION} EXACT REQUIRED COMPONENTS Development)
else()
find_package (Python COMPONENTS Development)
endif()

include_directories("${Python_INCLUDE_DIRS}")

add_library(deadparrot SHARED
    call.c
    eval.c
    unicode.c
)

install(TARGETS deadparrot DESTINATION lib)
install(FILES deadparrot.h DESTINATION include)
