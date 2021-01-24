function(add_project name)
    file(GLOB_RECURSE SRC "${name}/*.cpp" "${name}/*.h")

    add_executable(${name} ${SRC})
    set_target_properties(${name} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
    set_target_properties(${name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/media)

    # Setup includes and link
    target_include_directories(${name} PRIVATE ../include)
    target_link_libraries(${name} PRIVATE polygine)
endfunction()

add_project(terrain_editor)