cmake_minimum_required(VERSION 3.13)
project(polygine)

# Version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 3)
set(VERSION_STR "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set(POLY_BUILD_EXAMPLES FALSE CACHE BOOL "Set TRUE to build Polygine examples")
set(POLY_BUILD_TESTS FALSE CACHE BOOL "Set TRUE to build Polygine tests")
set(POLY_BUILD_UTIL TRUE CACHE BOOL "Set TRUE to build Polygine utility applications")
set(POLY_INSTALL_DIR ${CMAKE_INSTALL_PREFIX} CACHE PATH "Location to install Polygine headers and binaries")
set(POLY_ENABLE_PROFILING TRUE CACHE BOOL "Set TRUE to enable profiling")
set(POLY_COLUMN_MAJOR TRUE CACHE BOOL "Set TRUE to use column major matrices")

set(CMAKE_DEBUG_POSTFIX "-d")
set(CMAKE_INSTALL_PREFIX ${POLY_INSTALL_DIR})

# Compiler flags
if (MSVC)
    # Remove warnings
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
endif()

if (POLY_ENABLE_PROFILING)
    add_definitions(-DENABLE_PROFILING)
endif()

if (POLY_COLUMN_MAJOR)
    add_definitions(-DUSE_COLUMN_MAJOR)
endif()

# Build
add_subdirectory(src)
add_subdirectory(extlibs)

# Install
install(TARGETS polygine DESTINATION "${CMAKE_INSTALL_LIBDIR}" EXPORT polygine)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/poly" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(FILES license.md DESTINATION "share/doc/polygine")
install(FILES readme.md DESTINATION "share/doc/polygine")

# Setup for find_package
# set(POLY_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")
# set(POLY_LIB_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
# configure_file(
#     cmake/polygine-config.cmake.in
#     "${PROJECT_BINARY_DIR}/polygine-config.cmake" @ONLY)
# install(
#     FILES "${PROJECT_BINARY_DIR}/polygine-config.cmake"
#     DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/polygine")

# configure_file(
#     cmake/polygine-config-version.cmake.in
#     "${PROJECT_BINARY_DIR}/polygine-config-version.cmake" @ONLY)
# install(
#     FILES "${PROJECT_BINARY_DIR}/polygine-config-version.cmake"
#     DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/polygine")

# Export
# install(EXPORT polygine DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/polygine")


# Build examples
if (POLY_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Build tests
if (POLY_BUILD_TESTS)
    add_subdirectory(test)
endif()

# Build utility
if (POLY_BUILD_UTIL)
    add_subdirectory(util)
endif()