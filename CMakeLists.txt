cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/Source)
set(GUI_DIR ${CMAKE_CURRENT_LIST_DIR}/GUI/Source)
set(GLEW_DIR ${CMAKE_CURRENT_LIST_DIR}/3rdParty/glew-2.1.0)

file(GLOB SRC_CPP ${SRC_DIR}/*.cpp)
file(GLOB GUI_CPP ${GUI_DIR}/*.cpp)

include_directories(${SRC_DIR} ${GUI_DIR} ${GLEW_DIR}/include)
link_directories(${GLEW_DIR}/lib/${CMAKE_VS_PLATFORM_NAME})

add_subdirectory(Samples/Clouds  Clouds)
add_subdirectory(Samples/Shapes  Shapes)
add_subdirectory(Samples/Shaders Shaders)