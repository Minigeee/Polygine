include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/extlibs/cache")

CPMAddPackage("gh:catchorg/Catch2#devel")

# Add catch includes
set(CATCH_INCLUDE_DIR "${Catch2_SOURCE_DIR}/single_include/catch2")

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