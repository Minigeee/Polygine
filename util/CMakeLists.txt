function(add_project name)
    file(GLOB_RECURSE SRC "${name}/*.cpp" "${name}/*.h")

    add_executable(${name} ${SRC})
    set_target_properties(${name} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
    set_target_properties(${name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/media)

    # Setup includes and link
    target_include_directories(${name} PRIVATE ../include)
    target_link_libraries(${name} PRIVATE polygine)
endfunction()

set(PNG_INCLUDE_DIR "" CACHE PATH "libpng include directory")
set(PNG_LIBRARY_DEBUG "" CACHE FILEPATH "libpng debug binary")
set(PNG_LIBRARY_RELEASE "" CACHE FILEPATH "libpng release binary")

add_project(terrain_editor)
target_include_directories(terrain_editor PRIVATE ${PNG_INCLUDE_DIR})
target_link_libraries(terrain_editor PRIVATE debug ${PNG_LIBRARY_DEBUG})
target_link_libraries(terrain_editor PRIVATE optimized ${PNG_LIBRARY_RELEASE})