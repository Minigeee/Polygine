include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

# Setup cache
set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/extlibs/cache")

# Add installation dir to prefix paths
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}")

# SFML
find_package(SFML COMPONENTS audio network system QUIET)
if (SFML_FOUND)
    message(STATUS "SFML ${SFML_VERSION} found in ${SFML_DIR}")
    target_link_libraries(polygine sfml-system sfml-audio sfml-network)
elseif (POLY_DOWNLOAD_MISSING_DEPS)
    CPMAddPackage("gh:SFML/SFML#master")
    target_link_libraries(polygine PUBLIC sfml-system sfml-audio sfml-network)
    install(TARGETS sfml-system sfml-audio sfml-network OpenAL VORBIS FLAC EXPORT polygine-export)
    install(DIRECTORY ${SFML_SOURCE_DIR}/include/SFML DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
else()
    message("SFML not found")
endif()

# Hopscotch Map
find_package(tsl-hopscotch-map QUIET)
if (tsl-hopscotch-map_FOUND)
    target_link_libraries(polygine tsl::hopscotch_map)
elseif (POLY_DOWNLOAD_MISSING_DEPS)
    CPMAddPackage("gh:Tessil/hopscotch-map#master")
    target_link_libraries(polygine PUBLIC tsl::hopscotch_map)
    install(TARGETS hopscotch_map EXPORT polygine-export)
    install(DIRECTORY ${hopscotch-map_SOURCE_DIR}/include/tsl DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(FILES ${hopscotch-map_SOURCE_DIR}/tsl-hopscotch-map.natvis DESTINATION ${CMAKE_INSTALL_DATAROOTDIR})
else()
    message("tsl::hopscotch_map not found")
endif()

# GLFW
find_package(glfw3 QUIET)
if (glfw3_FOUND)
    message(STATUS "GLFW ${glfw3_VERSION} found in ${glfw3_DIR}")
    target_link_libraries(polygine glfw)
elseif (POLY_DOWNLOAD_MISSING_DEPS)
    CPMAddPackage("gh:glfw/glfw#master")
    target_link_libraries(polygine PRIVATE glfw)
    install(DIRECTORY ${glfw_SOURCE_DIR}/include/GLFW DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(TARGETS glfw EXPORT polygine-export)
else()
    message("GLFW not found")
endif()

# Assimp
find_package(assimp 5.0.0 QUIET)
if (assimp_FOUND)
    message(STATUS "Assimp ${assimp_VERSION} found in ${assimp_DIR}")
    target_link_libraries(polygine glfw)
elseif (POLY_DOWNLOAD_MISSING_DEPS)
    CPMAddPackage("gh:assimp/assimp#v5.0.1")
    target_link_libraries(polygine PRIVATE assimp)
    install(TARGETS zlibstatic IrrXML assimp EXPORT polygine-export)
else()
    message("Assimp not found")
endif()

# Freetype
find_package(freetype QUIET)
if (freetype_FOUND)
    message(STATUS "Freetype ${freetype_VERSION} found in ${freetype_DIR}")
    target_link_libraries(polygine freetype)
elseif (POLY_DOWNLOAD_MISSING_DEPS)
    CPMAddPackage("gh:freetype/freetype#master")
    target_link_libraries(polygine PRIVATE freetype)
    install(TARGETS freetype EXPORT polygine-export)
else()
    message("Freetype not found")
endif()

# Glad
add_library(glad INTERFACE)
target_include_directories(glad INTERFACE $<INSTALL_INTERFACE:include>)
install(TARGETS glad EXPORT polygine-export)

target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extlibs/glad/include>
    $<INSTALL_INTERFACE:include>)
target_sources(polygine PRIVATE ${CMAKE_SOURCE_DIR}/extlibs/glad/src/glad.c)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/extlibs/glad/include/glad DESTINATION DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/extlibs/glad/include/KHR DESTINATION DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Stb
CPMAddPackage("gh:nothings/stb#master")
target_include_directories(polygine PRIVATE ${stb_SOURCE_DIR})

# Fastnoise
CPMAddPackage("gh:Auburn/FastNoiseLite#master")
target_include_directories(polygine PRIVATE ${FastNoiseLite_SOURCE_DIR}/Cpp)

# RapidXML
target_include_directories(polygine PRIVATE ${CMAKE_SOURCE_DIR}/extlibs/rapidxml/include)


# ReactPhysics3D
CPMAddPackage("gh:DanielChappuis/reactphysics3d#master")
target_link_libraries(polygine PRIVATE reactphysics3d)
configure_file("${CMAKE_SOURCE_DIR}/extlibs/reactphysics3d/Collider.cpp" "${reactphysics3d_SOURCE_DIR}/src/collision/Collider.cpp")
configure_file("${CMAKE_SOURCE_DIR}/extlibs/reactphysics3d/PhysicsWorld.h" "${reactphysics3d_SOURCE_DIR}/include/reactphysics3d/engine/PhysicsWorld.h")
configure_file("${CMAKE_SOURCE_DIR}/extlibs/reactphysics3d/CollisionBody.h" "${reactphysics3d_SOURCE_DIR}/include/reactphysics3d/body/CollisionBody.h")
configure_file("${CMAKE_SOURCE_DIR}/extlibs/reactphysics3d/RigidBody.h" "${reactphysics3d_SOURCE_DIR}/include/reactphysics3d/body/RigidBody.h")
configure_file("${CMAKE_SOURCE_DIR}/extlibs/reactphysics3d/Components.h" "${reactphysics3d_SOURCE_DIR}/include/reactphysics3d/components/Components.h")
configure_file("${CMAKE_SOURCE_DIR}/extlibs/reactphysics3d/ColliderComponents.h" "${reactphysics3d_SOURCE_DIR}/include/reactphysics3d/components/ColliderComponents.h")
configure_file("${CMAKE_SOURCE_DIR}/extlibs/reactphysics3d/CollisionBodyComponents.h" "${reactphysics3d_SOURCE_DIR}/include/reactphysics3d/components/CollisionBodyComponents.h")
configure_file("${CMAKE_SOURCE_DIR}/extlibs/reactphysics3d/RigidBodyComponents.h" "${reactphysics3d_SOURCE_DIR}/include/reactphysics3d/components/RigidBodyComponents.h")
configure_file("${CMAKE_SOURCE_DIR}/extlibs/reactphysics3d/TransformComponents.h" "${reactphysics3d_SOURCE_DIR}/include/reactphysics3d/components/TransformComponents.h")
install(TARGETS reactphysics3d EXPORT polygine-export)

#[[
set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/extlibs/cache")

# ReactPhysics3D
CPMAddPackage("gh:DanielChappuis/reactphysics3d#master")
target_link_libraries(polygine reactphysics3d)
configure_file("${CMAKE_SOURCE_DIR}/extlibs/reactphysics3d/PhysicsWorld.h" "${reactphysics3d_SOURCE_DIR}/include/reactphysics3d/engine/PhysicsWorld.h")
configure_file("${CMAKE_SOURCE_DIR}/extlibs/reactphysics3d/CollisionBody.h" "${reactphysics3d_SOURCE_DIR}/include/reactphysics3d/body/CollisionBody.h")
configure_file("${CMAKE_SOURCE_DIR}/extlibs/reactphysics3d/RigidBody.h" "${reactphysics3d_SOURCE_DIR}/include/reactphysics3d/body/RigidBody.h")
configure_file("${CMAKE_SOURCE_DIR}/extlibs/reactphysics3d/Components.h" "${reactphysics3d_SOURCE_DIR}/include/reactphysics3d/components/Components.h")
configure_file("${CMAKE_SOURCE_DIR}/extlibs/reactphysics3d/ColliderComponents.h" "${reactphysics3d_SOURCE_DIR}/include/reactphysics3d/components/ColliderComponents.h")
configure_file("${CMAKE_SOURCE_DIR}/extlibs/reactphysics3d/CollisionBodyComponents.h" "${reactphysics3d_SOURCE_DIR}/include/reactphysics3d/components/CollisionBodyComponents.h")
configure_file("${CMAKE_SOURCE_DIR}/extlibs/reactphysics3d/RigidBodyComponents.h" "${reactphysics3d_SOURCE_DIR}/include/reactphysics3d/components/RigidBodyComponents.h")
configure_file("${CMAKE_SOURCE_DIR}/extlibs/reactphysics3d/TransformComponents.h" "${reactphysics3d_SOURCE_DIR}/include/reactphysics3d/components/TransformComponents.h")
target_include_directories(polygine PRIVATE ${reactphysics3d_SOURCE_DIR}/include)
if(NOT DEFINED reactphysics3d_FOUND)
    install(TARGETS reactphysics3d DESTINATION "${CMAKE_INSTALL_LIBDIR}" EXPORT polygine-export)
endif()

# Hopscotch Map
CPMAddPackage("gh:Tessil/hopscotch-map#master")
target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${hopscotch-map_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
install(DIRECTORY ${hopscotch-map_SOURCE_DIR}/include/tsl DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# GLFW
CPMAddPackage("gh:glfw/glfw#master")
target_link_libraries(polygine glfw)
target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${glfw_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
if(NOT DEFINED glfw_FOUND)
    install(TARGETS glfw DESTINATION "${CMAKE_INSTALL_LIBDIR}" EXPORT polygine-export)
endif()

# Assimp
CPMAddPackage("gh:assimp/assimp#v5.0.1")
target_link_libraries(polygine assimp)
target_include_directories(polygine PRIVATE ${assimp_SOURCE_DIR}/include)
if(NOT DEFINED assimp_FOUND)
    install(TARGETS zlibstatic IrrXML assimp DESTINATION "${CMAKE_INSTALL_LIBDIR}" EXPORT polygine-export)
endif()

# Freetype
CPMAddPackage("gh:freetype/freetype#master")
target_link_libraries(polygine freetype)
target_include_directories(polygine PRIVATE ${freetype_SOURCE_DIR}/include)
if(NOT DEFINED freetype_FOUND)
    install(TARGETS freetype DESTINATION "${CMAKE_INSTALL_LIBDIR}" EXPORT polygine-export)
endif()

# SFML
CPMAddPackage("gh:SFML/SFML#master")
set(SFML_BUILD_GRAPHICS FALSE)
set(SFML_BUILD_WINDOW FALSE)
target_link_libraries(polygine sfml-system sfml-audio sfml-network)
target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${SFML_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
if(NOT DEFINED SFML_FOUND)
    install(TARGETS sfml-system sfml-audio sfml-network DESTINATION "${CMAKE_INSTALL_LIBDIR}" EXPORT polygine-export)
    install(DIRECTORY ${SFML_SOURCE_DIR}/include/SFML/Audio DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SFML)
    install(DIRECTORY ${SFML_SOURCE_DIR}/include/SFML/Network DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SFML)
    install(DIRECTORY ${SFML_SOURCE_DIR}/include/SFML/System DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SFML)
endif()

# Stb
CPMAddPackage("gh:nothings/stb#master")
target_include_directories(polygine PRIVATE ${stb_SOURCE_DIR})

# Fastnoise
CPMAddPackage("gh:Auburn/FastNoiseLite#master")
target_include_directories(polygine PRIVATE ${FastNoiseLite_SOURCE_DIR}/Cpp)

# Glad
target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extlibs/glad/include>
    $<INSTALL_INTERFACE:include>)
target_sources(polygine PRIVATE ${CMAKE_SOURCE_DIR}/extlibs/glad/src/glad.c)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/extlibs/glad/include/glad DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/extlibs/glad/include/KHR DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# RapidXML
target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extlibs/rapidxml/include>
    $<INSTALL_INTERFACE:include>)
]]