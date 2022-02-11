cmake_minimum_required(VERSION 3.10)
project(Shaders)

add_compile_definitions(WIN32 _WINDOWS _UNICODE UNICODE)
add_link_options(/SUBSYSTEM:WINDOWS)

add_executable(${PROJECT_NAME}
               ${GUI_CPP}
               ${SRC_CPP}
               Application.cpp
               Viewport.cpp
               Program.cpp
               Triangle.cpp)
target_link_libraries(${PROJECT_NAME} opengl32.lib glu32.lib glew32.lib)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${GLEW_DIR}/bin/${CMAKE_VS_PLATFORM_NAME}/glew32.dll" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/"
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/VertexShader.txt" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/"
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/FragmentShader0.txt" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/"
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/FragmentShader1.txt" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/")