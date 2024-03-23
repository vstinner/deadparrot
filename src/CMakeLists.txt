cmake_minimum_required(VERSION 3.5)

project(deadparrot
        VERSION 0.0
        LANGUAGES C
        DESCRIPTION "use old removed Python C functions on new Python versions")

set(Python_VERSION "" CACHE STRING "Optional user-selected Python version")
set(Python_INCLUDE_DIR "" CACHE STRING "Optional user-selected Python include directory")

if(Python_INCLUDE_DIR)
  include_directories(${Python_INCLUDE_DIR})
  # link_directories is only needed on Windows
else()
  if(Python_VERSION)
    find_package (Python ${Python_VERSION} EXACT REQUIRED COMPONENTS Development)
    include_directories(${Python_INCLUDE_DIRS})
    link_directories(${Python_LIBRARY_DIRS})
  else()
    find_package (Python COMPONENTS Development)
    include_directories(${Python_INCLUDE_DIRS})
    link_directories(${Python_LIBRARY_DIRS})
  endif()
endif()

# On Windows, not only create .dll but also .lib
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(${PROJECT_NAME} SHARED
    call.c
    code.c
    dict.c
    eval.c
    float.c
    frame.c
    gc.c
    hash.c
    import.c
    interp.c
    list.c
    long.c
    module.c
    object.c
    tstate.c
    unicode.c
    weakref.c
)
set_target_properties(${PROJECT_NAME}
    PROPERTIES VERSION ${PROJECT_VERSION}
    PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR}
    PROPERTIES PUBLIC_HEADER deadparrot.h)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
