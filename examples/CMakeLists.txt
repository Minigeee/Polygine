function(add_example name src)
    add_executable(${name} ${src})
    set_target_properties(${name} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

    # Setup includes and link
    target_include_directories(${name} PRIVATE ../include)
    target_link_libraries(${name} PRIVATE polygine)

    # Install
    install(TARGETS ${name} DESTINATION ${POLY_INSTALL_DIR}/examples)
endfunction()

add_example(demo_app "demo_app/DemoApp.cpp")