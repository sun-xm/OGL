cmake_minimum_required(VERSION 3.10)
project(OGL)

# Config "cmake.configureSettings": { "GLEW_SHARED": "ON" } in settings.json to enable dynamic link to glew32.dll
set(GLEW_SHARED OFF CACHE BOOL "Link glew shared library")
if(GLEW_SHARED)
    set(GLEW_LIB "glew32")
else()
    set(GLEW_LIB "glew32s")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4099")
    add_compile_definitions(GLEW_STATIC)
    add_link_options(/NODEFAULTLIB:LIBCMT)
endif()

set(CMAKE_CXX_STANDARD 11)
set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/Source)
set(GUI_DIR ${CMAKE_CURRENT_LIST_DIR}/GUI/Source)
set(GLEW_DIR ${CMAKE_CURRENT_LIST_DIR}/3rdParty/glew-2.1.0)

file(GLOB SRC_CPP ${SRC_DIR}/*.cpp)
file(GLOB GUI_CPP ${GUI_DIR}/*.cpp)

include_directories(${SRC_DIR} ${GUI_DIR} ${GLEW_DIR}/include)
link_directories(${GLEW_DIR}/lib/${CMAKE_VS_PLATFORM_NAME})

add_compile_definitions(WIN32 _WINDOWS _UNICODE UNICODE)

add_subdirectory(Samples/Clouds  Clouds)
add_subdirectory(Samples/Shapes  Shapes)
add_subdirectory(Samples/Shaders Shaders)
add_subdirectory(Samples/Images  Images)