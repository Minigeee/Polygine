# Get Catch2
find_package(Catch2 QUIET)
if (NOT CATCH2_FOUND AND NOT EXISTS "${PROJECT_SOURCE_DIR}/extlibs/Catch2/CMakeLists.txt")
        message(STATUS "Getting Catch2...")

    execute_process(COMMAND git submodule update --init -- ../extlibs/Catch2
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
    message(STATUS "Found Catch2")
endif()

# Add catch includes
set(CATCH_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/extlibs/Catch2/single_include/catch2")

# Add tests
function(add_test name src)
    # Create executable
    add_executable(${name} ${src})
    set_target_properties(${name} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

    # Include dirs
    target_include_directories(${name} PRIVATE ${CATCH_INCLUDE_DIR})
    target_include_directories(${name} PRIVATE ${PROJECT_SOURCE_DIR}/include)

    # Link
    target_link_libraries(${name} PRIVATE polygine)
endfunction()

add_test(core_test "Core.cpp")
add_test(math_test "Math.cpp")