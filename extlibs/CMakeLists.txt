function(add_dependency name target src)
    # Find package
    find_package(${name} QUIET)

    # If not found, get submodule
    if (NOT &{name}_FOUND AND NOT EXISTS "${PROJECT_SOURCE_DIR}/extlibs/${src}/CMakeLists.txt")
        message(STATUS "Getting ${name}...")

        # Get submodule
        execute_process(COMMAND git submodule update --init -- "${PROJECT_SOURCE_DIR}/extlibs/${src}"
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    else()
        message(STATUS "Found ${name}")
    endif()

    if (NOT &{name}_FOUND)
        set(${name}_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/extlibs/${src}/include")
    endif()
    
    # Add project
    add_subdirectory("${src}")

    # Includes
    target_include_directories(polygine PUBLIC
        $<BUILD_INTERFACE:${${name}_INCLUDE_DIRS}>
        $<INSTALL_INTERFACE:include>)

    # Link
    target_link_libraries(polygine PUBLIC ${target})

    # Export
    install(TARGETS ${target} DESTINATION "${CMAKE_INSTALL_LIBDIR}" EXPORT ${target})

endfunction()

# Cmake dependencies
add_dependency(tsl-hopscotch-map hopscotch_map hopscotch-map)
add_dependency(tsl-sparse-map sparse_map sparse-map)