cmake_minimum_required(VERSION 3.13)

# Version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 5)
set(VERSION_STR "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# Project
project(polygine VERSION ${VERSION_STR})

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(POLY_BUILD_EXAMPLES FALSE CACHE BOOL "Set TRUE to build Polygine examples")
set(POLY_BUILD_TESTS FALSE CACHE BOOL "Set TRUE to build Polygine tests")
set(POLY_BUILD_UTIL TRUE CACHE BOOL "Set TRUE to build Polygine utility applications")
set(POLY_ENABLE_PROFILING TRUE CACHE BOOL "Set TRUE to enable profiling")
set(POLY_COLUMN_MAJOR TRUE CACHE BOOL "Set TRUE to use column major matrices")
set(POLY_DOWNLOAD_MISSING_DEPS FALSE CACHE BOOL "Set to TRUE to download any missing external dependencies")

set(CMAKE_DEBUG_POSTFIX "-d")
set(BUILD_SHARED_LIBS OFF)

# Compiler flags
if (MSVC)
    # Remove warnings
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
endif()

# Build
add_subdirectory(src)
add_subdirectory(extlibs)

if (POLY_ENABLE_PROFILING)
    target_compile_definitions(polygine PUBLIC ENABLE_PROFILING)
endif()

if (POLY_COLUMN_MAJOR)
    target_compile_definitions(polygine PUBLIC USE_COLUMN_MAJOR)
endif()

# Install
install(TARGETS polygine DESTINATION "${CMAKE_INSTALL_LIBDIR}" EXPORT polygine-export)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/poly" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(FILES license.md DESTINATION "share/doc/polygine")
install(FILES readme.md DESTINATION "share/doc/polygine")

include(CMakePackageConfigHelpers)
set(INSTALL_CONFIG_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/polygine")

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/polygine/PolygineConfigVersion.cmake"
    VERSION ${VERSION_STR}
    COMPATIBILITY SameMajorVersion
)

export(EXPORT polygine-export FILE "${CMAKE_CURRENT_BINARY_DIR}/polygine/PolygineTargets.cmake")

configure_package_config_file("cmake/PolygineConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/polygine/PolygineConfig.cmake"
    INSTALL_DESTINATION "${INSTALL_CONFIG_DIR}")

install(EXPORT polygine-export
    FILE PolygineTargets.cmake
    DESTINATION "${INSTALL_CONFIG_DIR}"
)
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/polygine/PolygineConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/polygine/PolygineConfigVersion.cmake"
    DESTINATION "${INSTALL_CONFIG_DIR}"
    COMPONENT devel
)


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