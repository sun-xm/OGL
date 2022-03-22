cmake_minimum_required(VERSION 3.10)
project(Shapes)

add_compile_definitions(WIN32 _WINDOWS _UNICODE UNICODE)
add_link_options(/SUBSYSTEM:WINDOWS)

add_executable(${PROJECT_NAME}
               ${GUI_CPP}
               ${SRC_CPP}
               Application.cpp
               ViewPort.cpp
               Panel.cpp
               Texture.cpp
               Triangle.cpp
               Square.cpp
               Plate.cpp
               Cone.cpp
               Cube.cpp
               Cylinder.cpp
               Sphere.cpp
               Donut.cpp
               Shapes.rc)
target_link_libraries(${PROJECT_NAME} opengl32.lib glu32.lib glew32.lib gdiplus.lib)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GLEW_DIR}/bin/${CMAKE_VS_PLATFORM_NAME}/glew32.dll" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/"
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/Portrait.png" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/")