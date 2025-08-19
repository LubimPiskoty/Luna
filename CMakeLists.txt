cmake_minimum_required(VERSION 3.20)
project(luna LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glm CONFIG REQUIRED)

add_executable(luna src/main.cpp)

target_link_libraries(luna PRIVATE glm::glm)
target_include_directories(luna PRIVATE include)