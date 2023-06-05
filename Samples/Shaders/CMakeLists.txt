cmake_minimum_required(VERSION 3.10)
project(Shaders)

file(GLOB APP_CPP ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

add_executable(${PROJECT_NAME} WIN32
               ${SRC_CPP}
               ${APP_CPP})
target_link_libraries(${PROJECT_NAME} W32LIB opengl32 glu32 ${GLEW_LIB})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/vshader.glsl" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/"
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/fshader.glsl" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/")

if(GLEW_SHARED)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       "${GLEW_DIR}/bin/${CMAKE_VS_PLATFORM_NAME}/glew32.dll"
                       "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/")
endif()