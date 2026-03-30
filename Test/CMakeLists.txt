project(Test)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../Source)

add_executable(Range Range.cpp)
add_executable(Stack Stack.cpp)

add_test(NAME Range COMMAND Range)
add_test(NAME Stack COMMAND Stack)