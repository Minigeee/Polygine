cmake_minimum_required(VERSION 3.1)
project(polygine)

# Version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)

set(POLY_BUILD_EXAMPLES FALSE CACHE BOOL "Set TRUE to build Polygine examples")
set(POLY_BUILD_TESTS FALSE CACHE BOOL "Set TRUE to build Polygine tests")
set(POLY_INSTALL_DIR ${CMAKE_INSTALL_PREFIX} CACHE PATH "Location to install Polygine headers and binaries")
set(POLY_ENABLE_PROFILING TRUE CACHE BOOL "Set TRUE to enable profiling")

set(CMAKE_DEBUG_POSTFIX "-d")

# Compiler flags
if (MSVC)
    # Remove warnings
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
endif()

if (POLY_ENABLE_PROFILING)
    add_definitions(-DENABLE_PROFILING)
endif()

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INC_DIR ${PROJECT_SOURCE_DIR}/include)

# Add sources
file(GLOB AUDIO_SRC
    ${SRC_DIR}/Audio/*.cpp
    ${INC_DIR}/poly/Audio/*.h)
file(GLOB CORE_SRC
    ${SRC_DIR}/Core/*.cpp
    ${INC_DIR}/poly/Core/*.h
    ${INC_DIR}/poly/Core/*.inl)
file(GLOB ENGINE_SRC
    ${SRC_DIR}/Engine/*.cpp
    ${INC_DIR}/poly/Engine/*.h)
file(GLOB GRAPHICS_SRC
    ${SRC_DIR}/Graphics/*.cpp
    ${INC_DIR}/poly/Graphics/*.h)
file(GLOB MATH_SRC
    ${SRC_DIR}/Math/*.cpp
    ${INC_DIR}/poly/Math/*.h
    ${INC_DIR}/poly/Math/*.inl)
file(GLOB NETWORK_SRC
    ${SRC_DIR}/Network/*.cpp
    ${INC_DIR}/poly/Network/*.h)
file(GLOB PHYSICS_SRC
    ${SRC_DIR}/Physics/*.cpp
    ${INC_DIR}/poly/Physics/*.h)

source_group("Audio" FILES ${AUDIO_SRC})
source_group("Core" FILES ${CORE_SRC})
source_group("Engine" FILES ${ENGINE_SRC})
source_group("Graphics" FILES ${GRAPHICS_SRC})
source_group("Math" FILES ${MATH_SRC})
source_group("Network" FILES ${NETWORK_SRC})
source_group("Physics" FILES ${PHYSICS_SRC})
    
# Build library
include_directories(include)
add_library(polygine STATIC
    ${AUDIO_SRC}
    ${CORE_SRC}
    ${ENGINE_SRC}
    ${GRAPHICS_SRC}
    ${MATH_SRC}
    ${NETWORK_SRC}
    ${PHYSICS_SRC})

# Install
install(TARGETS polygine DESTINATION ${POLY_INSTALL_DIR}/lib)
install(DIRECTORY ${INC_DIR} DESTINATION ${POLY_INSTALL_DIR})


# Build examples
if (POLY_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Build tests
if (POLY_BUILD_TESTS)
    add_subdirectory(test)
endif()