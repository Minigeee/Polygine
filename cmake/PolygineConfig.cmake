include(CMakeFindDependencyMacro)

find_dependency(glfw REQUIRED)
find_dependency(assimp REQUIRED)
find_dependency(freetype REQUIRED)

set(polygine_target_file "${CMAKE_CURRENT_LIST_FILE}/PolygineTargets.cmake" PATH)

if(NOT TARGET polygine)
    set(polygine_FOUND TRUE)
    include("${polygine_target_file}")
endif()