cmake_minimum_required(VERSION 3.10)

include(CTest)
include(Catch)

find_package(Catch2 3 REQUIRED)

include_directories(../include)

file(GLOB SOURCES */*.cpp)

add_executable(Tests ${SOURCES})
target_link_libraries(Tests PRIVATE Catch2::Catch2)

catch_discover_tests(Tests)