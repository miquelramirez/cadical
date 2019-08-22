cmake_minimum_required(VERSION 3.10)
project(CaDiCaL)

set(PROJECT_VERSION_MAJOR "1")
set(PROJECT_VERSION_MINOR "0")
set(PROJECT_VERSION_PATCH "0")

set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(LIBRARY_VERSION ${PROJECT_VERSION})
set(LIBRARY_SOVERSION ${PROJECT_VERSION_MAJOR})

set(CMAKE_CXX_STANDARD 17)

set(dir ${PROJECT_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${dir}/build")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${dir}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/build")

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)

configure_file(cadical.pc.in cadical.pc @ONLY)

#install(FILES ${PROJECT_BINARY_DIR}/build.hpp
#        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cadical)

install(FILES ${CMAKE_BINARY_DIR}/cadical.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# for configuration file
include_directories(${PROJECT_BINARY_DIR})
# for sources
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(src)
add_subdirectory(test/api)
