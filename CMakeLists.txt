cmake_minimum_required(VERSION 3.1)
project(polygine)

# Version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

set(PLG_BUILD_EXAMPLES OFF CACHE BOOL "Set TRUE to build Polygine examples")

# Add sources
file(GLOB SOURCES "src/*.cpp")

# Add examples
if (PLG_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()