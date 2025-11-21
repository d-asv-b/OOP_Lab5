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

# Добавление тестов
enable_testing()

# Добавление тестов в тестовый набор
add_test(NAME Array_tests COMMAND Array_tests)
add_test(NAME Figures_tests COMMAND Figures_tests)