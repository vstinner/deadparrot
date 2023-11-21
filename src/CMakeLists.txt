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
include_directories(${Python_INCLUDE_DIRS})
link_directories(${Python_LIBRARY_DIRS})

add_library(${PROJECT_NAME} SHARED
    call.c
    eval.c
    unicode.c
)
set_target_properties(${PROJECT_NAME}
    PROPERTIES VERSION ${PROJECT_VERSION}
    PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR}
    PROPERTIES PUBLIC_HEADER deadparrot.h)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
