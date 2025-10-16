project(Shaders)

file(GLOB APP_SRC ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

add_executable(${PROJECT_NAME} WIN32 ${APP_SRC})

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
        VECTORX_NS=vx
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        opengl32
        glu32
        ${GLEW_LIB}
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/vshader.glsl" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/"
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/fshader.glsl" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/")

if(GLEW_SHARED)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       "${GLEW_DIR}/bin/${CMAKE_VS_PLATFORM_NAME}/glew32.dll"
                       "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/")
endif()