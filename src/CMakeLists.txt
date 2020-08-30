set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INC_DIR ${PROJECT_SOURCE_DIR}/include/poly)

# Add sources
file(GLOB AUDIO_SRC
    ${SRC_DIR}/Audio/*.cpp
    ${INC_DIR}/Audio/*.h
    ${INC_DIR}/Audio/*.inl)
file(GLOB CORE_SRC
    ${SRC_DIR}/Core/*.cpp
    ${INC_DIR}/Core/*.h
    ${INC_DIR}/Core/*.inl)
file(GLOB ENGINE_SRC
    ${SRC_DIR}/Engine/*.cpp
    ${INC_DIR}/Engine/*.h
    ${INC_DIR}/Engine/*.inl)
file(GLOB GRAPHICS_SRC
    ${SRC_DIR}/Graphics/*.cpp
    ${INC_DIR}/Graphics/*.h
    ${INC_DIR}/Graphics/*.inl)
file(GLOB MATH_SRC
    ${SRC_DIR}/Math/*.cpp
    ${INC_DIR}/Math/*.h
    ${INC_DIR}/Math/*.inl)
file(GLOB NETWORK_SRC
    ${SRC_DIR}/Network/*.cpp
    ${INC_DIR}/Network/*.h
    ${INC_DIR}/Network/*.inl)
file(GLOB PHYSICS_SRC
    ${SRC_DIR}/Physics/*.cpp
    ${INC_DIR}/Physics/*.h
    ${INC_DIR}/Physics/*.inl)

source_group("Audio" FILES ${AUDIO_SRC})
source_group("Core" FILES ${CORE_SRC})
source_group("Engine" FILES ${ENGINE_SRC})
source_group("Graphics" FILES ${GRAPHICS_SRC})
source_group("Math" FILES ${MATH_SRC})
source_group("Network" FILES ${NETWORK_SRC})
source_group("Physics" FILES ${PHYSICS_SRC})
    
# Build library
add_library(polygine STATIC
    ${AUDIO_SRC}
    ${CORE_SRC}
    ${ENGINE_SRC}
    ${GRAPHICS_SRC}
    ${MATH_SRC}
    ${NETWORK_SRC}
    ${PHYSICS_SRC})
    
target_include_directories(polygine PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)