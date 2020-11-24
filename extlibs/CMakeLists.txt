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

    # Link
    target_link_libraries(polygine PUBLIC ${target})

    # Export
    # install(TARGETS ${target} EXPORT polygine)
endfunction()

function(add_dependency_headers name src include_dir)
    # If not found, get submodule
    if (NOT EXISTS "${PROJECT_SOURCE_DIR}/extlibs/${src}")
        message(STATUS "Getting ${name}...")

        # Get submodule
        execute_process(COMMAND git submodule update --init -- "${PROJECT_SOURCE_DIR}/extlibs/${src}"
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    else()
        message(STATUS "Found ${name}")
    endif()
    
    # Add includes
    target_include_directories(polygine PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/extlibs/${src}/include>
        $<INSTALL_INTERFACE:include>)

    # Install headers
    install(DIRECTORY "${PROJECT_SOURCE_DIR}/extlibs/${src}/include/${include_dir}" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
endfunction()

# Cmake dependencies
add_dependency_headers(tsl-hopscotch-map hopscotch-map tsl)

add_dependency(glfw glfw glfw)
add_dependency(assimp assimp assimp)

# Glad
target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/extlibs/glad/include>
    $<INSTALL_INTERFACE:include>)
target_sources(polygine PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/glad/src/glad.c")
install(DIRECTORY "${PROJECT_SOURCE_DIR}/extlibs/glad/include/glad" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

# stb_image
if (NOT EXISTS "${PROJECT_SOURCE_DIR}/extlibs/stb/stb_image.h")
    message(STATUS "Getting stb_image.h...")

    # Get submodule
    execute_process(COMMAND git submodule update --init -- "${PROJECT_SOURCE_DIR}/extlibs/stb"
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
    message(STATUS "Found stb_image.h")
endif()

target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/extlibs/stb>
    $<INSTALL_INTERFACE:include>)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/extlibs/stb" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")