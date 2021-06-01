include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/extlibs/cache")
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
target_include_directories(polygine PRIVATE ${glfw_SOURCE_DIR}/include)
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

# RapidXML
target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extlibs/rapidxml/include>
    $<INSTALL_INTERFACE:include>)