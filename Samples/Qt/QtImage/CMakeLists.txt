cmake_minimum_required(VERSION 3.7)
project(QtImage)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

include_directories(.)

file(GLOB APP_SRC "${CMAKE_CURRENT_LIST_DIR}/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/*.ui" "${CMAKE_CURRENT_LIST_DIR}/*.qrc")

if(WIN32)
    set(Qt5_DIR "$ENV{Qt5_DIR}/lib/cmake/Qt5") # Example C:\Qt\Qt5.9.9\5.9.9\msvc2013_64\lib\cmake\Qt5
    list(APPEND OpenGL opengl32 glu32)
    add_executable(${PROJECT_NAME} WIN32
                   ${APP_SRC})

    set(SUFFIX "$<IF:$<CONFIG:Debug>,d,>")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{Qt5_DIR}/bin/Qt5Core${SUFFIX}.dll"    "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/"
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{Qt5_DIR}/bin/Qt5Gui${SUFFIX}.dll"     "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/"
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{Qt5_DIR}/bin/Qt5Widgets${SUFFIX}.dll" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/"
                       COMMAND ${CMAKE_COMMAND} -E make_directory    "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/platforms/"
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different "$ENV{Qt5_DIR}/plugins/platforms/qwindows${SUFFIX}.dll" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/platforms/")
else()
    add_executable(${PROJECT_NAME}
                   ${APP_SRC})
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(${PROJECT_NAME} ${OpenGL} Qt5::Widgets)