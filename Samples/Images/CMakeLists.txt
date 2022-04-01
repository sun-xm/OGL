cmake_minimum_required(VERSION 3.10)
project(Images)

add_executable(${PROJECT_NAME} WIN32
               ${GUI_CPP}
               ${SRC_CPP}
               Application.cpp
               Viewport.cpp
               Texture.cpp)
target_link_libraries(${PROJECT_NAME} opengl32 glu32 gdiplus ${GLEW_LIB})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/Portrait.png" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/"
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/VertexShader.glsl" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/"
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/FragmentShader.glsl" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/")

if(GLEW_SHARED)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       "${GLEW_DIR}/bin/${CMAKE_VS_PLATFORM_NAME}/glew32.dll"
                       "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/")
endif()