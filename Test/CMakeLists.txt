project(Test)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../Source)

add_executable(Test Test.cpp)

add_test(NAME Test COMMAND Test)