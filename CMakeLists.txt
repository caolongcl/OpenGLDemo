cmake_minimum_required(VERSION 3.9)
project(opengles)

set(CMAKE_CXX_STANDARD 14)

set(PROJECT opengles)
set(INCLUDE_DIR include)
set(SRC_DIR src)
set(LIB_DIR lib)

include_directories(${INCLUDE_DIR})
include_directories(${INCLUDE_DIR}/app)
include_directories(${INCLUDE_DIR}/core)
include_directories(${INCLUDE_DIR}/object)
include_directories(${INCLUDE_DIR}/third_party)

aux_source_directory(${SRC_DIR}/app SRC_APP)
aux_source_directory(${SRC_DIR}/core SRC_CORE)
aux_source_directory(${SRC_DIR}/object SRC_OBJECT)
aux_source_directory(${SRC_DIR}/third_party/glad SRC_GLAD)
aux_source_directory(${SRC_DIR}/third_party/stb SRC_STB)

link_directories(${LIB_DIR}/assimp)
link_directories(${LIB_DIR}/glfw)

set(SRC_FILES main.cpp)

add_executable(${PROJECT} ${SRC_FILES}
        ${SRC_APP} ${SRC_CORE} ${SRC_OBJECT} ${SRC_GLAD} ${SRC_STB})

target_link_libraries(${PROJECT}
        libglfw.so
        libassimp.so
        libdl.so
        libpthread.so)