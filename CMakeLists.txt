cmake_minimum_required(VERSION 3.10)

project(Lab5)

set(CMAKE_BUILD_TYPE Debug)

add_library(${PROJECT_NAME}_lib
  src/MemoryResource.cpp
)

add_executable(
    ${PROJECT_NAME}_exe main.cpp
)
set_target_properties(${PROJECT_NAME}_exe PROPERTIES OUTPUT_NAME "Lab_exe")

target_compile_options(${PROJECT_NAME}_exe PRIVATE -g)
target_link_libraries(${PROJECT_NAME}_exe PRIVATE ${PROJECT_NAME}_lib)

enable_testing()

add_executable(MemoryResource_tests
    test/memory_resource_test.cpp
)
add_executable(LinkedListBasic_tests
    test/linked_list_basic_test.cpp
)
add_executable(LinkedListOperations_tests
    test/linked_list_operations_test.cpp
)

target_link_libraries(MemoryResource_tests ${PROJECT_NAME}_lib gtest_main)
target_link_libraries(LinkedListBasic_tests ${PROJECT_NAME}_lib gtest_main)
target_link_libraries(LinkedListOperations_tests ${PROJECT_NAME}_lib gtest_main)


add_test(NAME MemoryResource_tests COMMAND MemoryResource_tests)
add_test(NAME LinkedListBasic_tests COMMAND LinkedListBasic_tests)
add_test(NAME LinkedListOperations_tests COMMAND LinkedListOperations_tests)