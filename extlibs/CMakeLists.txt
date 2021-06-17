include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

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

# Use local packages if available
set(CPM_USE_LOCAL_PACKAGES)

# Hopscotch Map
CPMAddPackage("gh:Tessil/hopscotch-map#master")
target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${hopscotch-map_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
install(DIRECTORY ${hopscotch-map_SOURCE_DIR}/include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# GLFW
CPMAddPackage("gh:glfw/glfw#master")
target_link_libraries(polygine glfw)
target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${glfw_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
if(NOT DEFINED glfw_FOUND)
    install(TARGETS glfw DESTINATION "${CMAKE_INSTALL_LIBDIR}" EXPORT polygine-export)
    install(DIRECTORY ${glfw_SOURCE_DIR}/include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
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
    install(DIRECTORY ${SFML_SOURCE_DIR}/include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
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
install(DIRECTORY ${CMAKE_SOURCE_DIR}/extlibs/glad/include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# RapidXML
target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extlibs/rapidxml/include>
    $<INSTALL_INTERFACE:include>)