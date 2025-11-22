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

add_executable(MemoryResource_tests
    test/memory_resource_test.cpp
)
target_compile_options(MemoryResource_tests PRIVATE -g)
target_link_libraries(MemoryResource_tests PRIVATE gtest gtest_main ${PROJECT_NAME}_lib)
add_test(NAME MemoryResource_tests COMMAND MemoryResource_tests)

add_executable(LinkedListBasic_tests
    test/linked_list_basic_test.cpp
)
target_compile_options(LinkedListBasic_tests PRIVATE -g)
target_link_libraries(LinkedListBasic_tests PRIVATE gtest gtest_main ${PROJECT_NAME}_lib)
add_test(NAME LinkedListBasic_tests COMMAND LinkedListBasic_tests)

add_executable(LinkedListOperations_tests
    test/linked_list_operations_test.cpp
)
target_compile_options(LinkedListOperations_tests PRIVATE -g)
target_link_libraries(LinkedListOperations_tests PRIVATE gtest gtest_main ${PROJECT_NAME}_lib)
add_test(NAME LinkedListOperations_tests COMMAND LinkedListOperations_tests)