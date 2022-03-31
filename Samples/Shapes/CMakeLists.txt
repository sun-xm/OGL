cmake_minimum_required(VERSION 3.10)
project(Shapes)

add_executable(${PROJECT_NAME} WIN32
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
target_link_libraries(${PROJECT_NAME} opengl32 glu32 gdiplus ${GLEW_LIB})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   "${CMAKE_CURRENT_LIST_DIR}/Portrait.png"
                   "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/")

if(GLEW_SHARED)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       "${GLEW_DIR}/bin/${CMAKE_VS_PLATFORM_NAME}/glew32.dll"
                       "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/")
endif()