cmake_minimum_required(VERSION 3.10)

find_package(Catch2 3)
if(NOT Catch2_FOUND)
    add_subdirectory(../3rdparty/Catch ../3rdparty/Catch/build)
endif()

file(GLOB SOURCES */*.cpp)

add_executable(Tests ${SOURCES})
target_include_directories(Tests ../include)
target_link_libraries(Tests PRIVATE Catch2::Catch2)

catch_discover_tests(Tests)