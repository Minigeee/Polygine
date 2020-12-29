function(add_example name)
    file(GLOB_RECURSE SRC "${name}/*.cpp" "${name}/*.h")

    add_executable(${name} ${SRC})
    set_target_properties(${name} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
    set_target_properties(${name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${name})

    # Setup includes and link
    target_include_directories(${name} PRIVATE ../include)
    target_link_libraries(${name} PRIVATE polygine)
endfunction()

add_example(demo_app)